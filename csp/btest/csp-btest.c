/**
 * @file csp-btest.h
 * Entry point of CSP lib's benchmarking program.
 *
 * @author Xibitol <xibitol@pimous.dev>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#include "util/unused.h"
#include "solve-queens.h"
#include "load-new-sudoku.h"
#include "solve-sudoku.h"

#define NQUEENS_RESULT_FILE "n_queens_benchmark.txt"
#define SUDOKU_RESULT_FILE "sudoku_benchmark.txt"

typedef int BenchmarkFunc(const char* resultFile, void* arg);

static int exitCode = EXIT_SUCCESS;

static pid_t benchmark(const char* resultFile, BenchmarkFunc* func, void* arg){
	pid_t fpid = -1;

	// Creates or truncates result file.
	FILE *file = fopen(resultFile, "w");
    if(file == NULL){
        perror("fopen"), exitCode = EXIT_FAILURE;
        return fpid;
    } else if(fclose(file) == -1) perror("fopen"), exitCode = EXIT_FAILURE;

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

static int nqueensBenchmark(const char* resultFile, void* arg){
	int test_count = *((int*) arg);

	for (int i = 4; i < test_count+4; i++) {
		solve_queens(i, resultFile, true);
	}

	return EXIT_SUCCESS;
}

static int sudokuBenchmark(const char* resultFile, void* arg){
	int average_amount = ((int*) arg)[0];
	int unknown_increment = ((int*) arg)[1];
	size_t** sudokus = 0;

	for (int i = 5; i < 81; i += unknown_increment) {
		sudokus = load_new_sudoku(i, average_amount);

		for (int j = 0; j < average_amount; j++) {
			solve_sudoku(sudokus[j], resultFile, true);
		}
	}

	free(sudokus);
	return EXIT_SUCCESS;
}

int main(void){
	int nqueensArgs[1] = {16};
	pid_t npid = benchmark(NQUEENS_RESULT_FILE, &nqueensBenchmark, nqueensArgs);
	printf("Started benchmarking on %d NQueens problems (%d).\n",
		nqueensArgs[0], npid
	);

	int sudokuArgs[2] = {5, 5};
	pid_t spid = benchmark(SUDOKU_RESULT_FILE, &sudokuBenchmark, sudokuArgs);
	printf("Started benchmarking on Sudoku puzzles (%d).\n", spid);

	if(npid != -1 && waitpid(npid, NULL, 0) == -1)
		perror("waitpid"), exitCode = EXIT_FAILURE;
	else{
		printf("Finished benchmarking (NQueens problems; %d).\n", getpid());
	}
	if(spid != -1 && waitpid(spid, NULL, 0) == -1)
		perror("waitpid"), exitCode = EXIT_FAILURE;
	else{
		printf("Finished benchmarking (Sudoku puzzles; %d).\n", getpid());
	}

	return EXIT_SUCCESS;
}