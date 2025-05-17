/**
 * @file csp-btest.c
 * Entry point of CSP lib's benchmarking program.
 *
 * @author Xibitol <xibitol@pimous.dev>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "csp-btest.h"

#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>

#include "util/unused.h"
#include "sudoku-generate.h"
#include "queens-solve.h"
#include "solve-sudoku.h"
#include "dumb-solve-sudoku.h"

// PRIVATE
static int exitCode = EXIT_SUCCESS;

// Type defs
typedef int BenchmarkFunc(const char* resultFile, void* arg);

// Functions
static pid_t benchmark(const char* resultFile, BenchmarkFunc* func, void* arg){
	pid_t fpid = -1;

	// Creates or truncates result file.
	FILE *file = fopen(resultFile, "w");
	if(file == NULL){
		perror("fopen"), exitCode = EXIT_FAILURE;
		return fpid;
	}else if(fclose(file) == -1)
		perror("fopen"), exitCode = EXIT_FAILURE;

	// Starts benchmark in new process.
	switch(fpid = fork()){
		case -1:
			perror("fork"), exitCode = EXIT_FAILURE;
			break;
		case 0:
			exit(func(resultFile, arg));
	}

	return fpid;
}

static int benchmark_nqueens(const char* resultFile, void* arg){
	unsigned int count = *((int*) arg);

	for(unsigned int i = 4; i < count + 4; i++)
		queens_solve(i, resultFile, true);

	return EXIT_SUCCESS;
}
static int benchmark_sudoku(const char* resultFile, void* arg){
	unsigned int average_amount = ((int*) arg)[0];
	unsigned int unknown_increment = ((int*) arg)[1];
	size_t** sudokus = 0;

	for(unsigned int i = 5; i < 81; i += unknown_increment){
		sudokus = sudoku_generate(average_amount, i);

		for(unsigned int j = 0; j < average_amount; j++){
			sudoku_solve(sudokus[j], resultFile, true);
			free(sudokus[j]);
		}

		free(sudokus);
	}

	return EXIT_SUCCESS;
}

// PUBLIC
// Functions
int main(void){
	// Starts benchmarks
	int nqueensArgs[1] = {16};
	pid_t npid = benchmark(NQUEENS_RESULT_FILE,
		&benchmark_nqueens, nqueensArgs
	);
	printf("Started benchmarking on %d NQueens problems (%d).\n",
		nqueensArgs[0], npid
	);

	int sudokuArgs[2] = {5, 5};
	pid_t spid = benchmark(SUDOKU_RESULT_FILE, &benchmark_sudoku, sudokuArgs);
	printf("Started benchmarking on Sudoku puzzles (%d).\n", spid);

	// Finishes benchmarks
	int stat;

	if(npid != -1 && waitpid(npid, &stat, 0) == -1)
		perror("waitpid"), exitCode = EXIT_FAILURE;
	else
		printf("Finished benchmarking on NQueens problems (exit: %d; %d).\n",
			WEXITSTATUS(stat), npid
		);

	if(spid != -1 && waitpid(spid, &stat, 0) == -1)
		perror("waitpid"), exitCode = EXIT_FAILURE;
	else
		printf("Finished benchmarking on Sudoku puzzles (exit: %d; %d).\n",
			WEXITSTATUS(stat), spid
		);

	return exitCode;
}