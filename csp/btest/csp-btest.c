/**
 * @file csp-btest.h
 * Entry point of CSP lib's benchmarking program.
 *
 * @author Xibitol <xibitol@pimous.dev>
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
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
#define NQUEENS_FC_RESULT_FILE "n_queens_fc_benchmark.txt"
#define SUDOKU_RESULT_FILE "sudoku_benchmark.txt"

typedef int BenchmarkFunc(const char* resultFile, void* arg);

typedef struct{
	int total_count;
	size_t** sudokus;
}SudokuArgs;

static int exitCode = EXIT_SUCCESS;

void save_sudokus_to_file(size_t** sudokus, int count, const char* filename) {
	FILE* file = fopen(filename, "ab");
	if (file == NULL) {
		perror("fopen");
		return;
	}

	for (int i = 0; i < count; i++) {
		fwrite(sudokus[i], sizeof(size_t), 81, file);
	}

	fclose(file);
}

size_t** load_sudokus_from_file(int count, const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (file == NULL) {
		perror("fopen");
		return NULL;
	}

	size_t** sudokus = malloc(count * sizeof(size_t*));
	if (sudokus == NULL) {
		perror("malloc");
		fclose(file);
		return NULL;
	}

	for (int i = 0; i < count; i++) {
		sudokus[i] = malloc(81 * sizeof(size_t));
		if (sudokus[i] == NULL) {
			perror("malloc");
			fclose(file);
			return NULL;
		}
		fread(sudokus[i], sizeof(size_t), 81, file);
	}
	fclose(file);
	return sudokus;
}

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
		solve_queens(i, resultFile, false, true);
	}

	return EXIT_SUCCESS;
}

static int nqueensFCBenchmark(const char* resultFile, void* arg){
	int test_count = *((int*) arg);

	for (int i = 4; i < test_count+4; i++) {
		solve_queens(i, resultFile, true, true);
	}

	return EXIT_SUCCESS;
}

static int sudokuBenchmark(const char* resultFile, void* arg) {
	const int total_count = ((SudokuArgs*) arg)->total_count;
	size_t** sudokus = ((SudokuArgs*) arg)->sudokus;

	for (int j = 0; j < total_count; j++) {
		if (solve_sudoku(sudokus[j], resultFile, false, true)) {
			perror("solve_sudoku");
		}
	}

	return EXIT_SUCCESS;
}

int main(void){
	int nqueensArgs[1] = {16};
	pid_t npid = benchmark(NQUEENS_RESULT_FILE, &nqueensBenchmark, nqueensArgs);
	printf("Started benchmarking on %d NQueens problems (%d).\n",
		nqueensArgs[0], npid
	);

	pid_t nfcpid = benchmark(NQUEENS_FC_RESULT_FILE, &nqueensFCBenchmark, nqueensArgs);
	printf("Started FC benchmarking on %d NQueens problems (%d).\n",
		nqueensArgs[0], nfcpid
	);

	int average_amount = 5;
	int increment = 5;

	SudokuArgs sudokuArgs = {0};
	sudokuArgs.sudokus = malloc(81 * sizeof(size_t*)); // Allocate memory for the flexible array
	if (sudokuArgs.sudokus == NULL) {
	    perror("malloc");
	    exit(EXIT_FAILURE);
	}

	int i = 5;
	while (i < 81) {
	    size_t** sudokus = load_new_sudoku(i, average_amount);
	    i += increment;
	    for (int j = 0; j < average_amount; j++) {
	        sudokuArgs.sudokus[sudokuArgs.total_count + j] = sudokus[j];
	    }
	    sudokuArgs.total_count += average_amount;
	    free(sudokus); // Free the temporary sudokus array
	}

	pid_t spid = benchmark(SUDOKU_RESULT_FILE, &sudokuBenchmark, &sudokuArgs);
	printf("Started benchmarking on Sudoku puzzles (%d).\n", spid);

	if(npid != -1 && waitpid(npid, NULL, 0) == -1)
		perror("waitpid"), exitCode = EXIT_FAILURE;
	else{
		printf("Finished benchmarking (NQueens problems; %d).\n", getpid());
	}
	if(nfcpid != -1 && waitpid(nfcpid, NULL, 0) == -1)
		perror("waitpid"), exitCode = EXIT_FAILURE;
	else{
		printf("Finished FC benchmarking (NQueens problems; %d).\n", getpid());
	}
	if(spid != -1 && waitpid(spid, NULL, 0) == -1)
		perror("waitpid"), exitCode = EXIT_FAILURE;
	else{
		printf("Finished benchmarking (Sudoku puzzles; %d).\n", getpid());
	}

	for (int i = 0; i < sudokuArgs.total_count; i++) {
		free(sudokuArgs.sudokus[i]);
	}
	free(sudokuArgs.sudokus);

	return EXIT_SUCCESS;
}