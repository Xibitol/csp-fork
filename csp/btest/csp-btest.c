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
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include <csp.h>
#include "util/unused.h"
#include "solve-queens.h"
#include "load-new-sudoku.h"
#include "solve-sudoku.h"

#define NQUEENS_RESULT_DIR "n_queens/"
#define NQUEENS_RESULT_FILE "n_queens/_basic_benchmark.txt"
#define NQUEENS_FC_RESULT_FILE "n_queens/_fc_benchmark.txt"
#define NQUEENS_FC_OVARS_MIN_RESULT_FILE "n_queens/_fc_ovars_min_benchmark.txt"
#define NQUEENS_FC_OVARS_MAX_RESULT_FILE "n_queens/_fc_ovars_max_benchmark.txt"
#define SUDOKU_RESULT_DIR "sudoku/"
#define SUDOKU_RESULT_FILE "sudoku/_basic_benchmark.txt"
#define SUDOKU_FC_RESULT_FILE "sudoku/_fc_benchmark.txt"
#define SUDOKU_FC_OVARS_MIN_RESULT_FILE "sudoku/_fc_ovars_min_benchmark.txt"
// #define SUDOKU_FC_OVARS_MAX_RESULT_FILE "n_queens/_fc_ovars_max_benchmark.txt"

typedef int BenchmarkFunc(const char* resultFile, void* arg);

typedef struct{
	int total_count;
	size_t** sudokus;
	SolveType solve_type;
}SudokuArgs;

typedef struct {
	int total_count;
	SolveType solve_type;
}NQueensArgs;

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
	int test_count = ((NQueensArgs*) arg)->total_count;
	SolveType solve_type = ((NQueensArgs*) arg)->solve_type;

	for (int i = 4; i < test_count+4; i++) {
		solve_queens(i, resultFile, solve_type, true);
	}

	return EXIT_SUCCESS;
}

static int sudokuBenchmark(const char* resultFile, void* arg) {
	const int total_count = ((SudokuArgs*) arg)->total_count;
	size_t** sudokus = ((SudokuArgs*) arg)->sudokus;
	SolveType solve_type = ((SudokuArgs*) arg)->solve_type;

	for (int j = 0; j < total_count; j++) {
		if (solve_sudoku(sudokus[j], resultFile, solve_type, true)) {
			perror("solve_sudoku");
		}
	}

	return EXIT_SUCCESS;
}

int main(void) {
	mkdir(NQUEENS_RESULT_DIR, 0733);
	mkdir(SUDOKU_RESULT_DIR, 0733);

	NQueensArgs nqueensArgsB = {20, 0};
	pid_t npid = benchmark(NQUEENS_RESULT_FILE,
		nqueensBenchmark, &nqueensArgsB
	);
	printf("Started benchmarking on %d NQueens problems (%d).\n",
		nqueensArgsB.total_count, npid
	);

	NQueensArgs nqueensArgsOV = {20, FC | OVARS_MIN};
	pid_t nfcovminpid = benchmark(NQUEENS_FC_OVARS_MIN_RESULT_FILE,
		nqueensBenchmark, &nqueensArgsOV
	);
	printf("Started FC_OVARS_MIN benchmarking on %d NQueens problems (%d).\n",
		nqueensArgsOV.total_count, nfcovminpid
	);

	NQueensArgs nqueensArgsFC = {20, FC};
	pid_t nfcpid = benchmark(NQUEENS_FC_RESULT_FILE,
		nqueensBenchmark, &nqueensArgsFC)
		;
	printf("Started FC benchmarking on %d NQueens problems (%d).\n",
		nqueensArgsFC.total_count, nfcpid
	);

	NQueensArgs nqueensArgsFCOV = {20, FC | OVARS_MAX};
	pid_t nfcovmaxpid = benchmark(NQUEENS_FC_OVARS_MAX_RESULT_FILE,
		nqueensBenchmark, &nqueensArgsFCOV
	);
	printf("Started FC_OVARS_MAX benchmarking on %d NQueens problems (%d).\n",
	  nqueensArgsFCOV.total_count, nfcovmaxpid
	);

	int average_amount = 5;
	int increment = 5;

	SudokuArgs sudokuArgs = {0};
	// Allocate memory for the flexible array
	sudokuArgs.sudokus = malloc(81 * sizeof(size_t*));
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

	sudokuArgs.solve_type = 0;
	pid_t spid = benchmark(SUDOKU_RESULT_FILE, &sudokuBenchmark, &sudokuArgs);
	printf("Started benchmarking on Sudoku puzzles (%d).\n", spid);

	// sudokuArgs.solve_type = FC | OVARS_MAX;
	// pid_t sfcovmaxpid = benchmark(SUDOKU_FC_OVARS_MAX_RESULT_FILE, &sudokuBenchmark, &sudokuArgs);
	// printf("Started FC_OVARS_MAX benchmarking on Sudoku puzzles (%d).\n", sfcovmaxpid);

	sudokuArgs.solve_type = FC;
	pid_t sfcpid = benchmark(SUDOKU_FC_RESULT_FILE,
		&sudokuBenchmark, &sudokuArgs
	);
	printf("Started FC benchmarking on Sudoku puzzles (%d).\n", sfcpid);

	sudokuArgs.solve_type = FC | OVARS_MIN;
	pid_t sfcovminpid = benchmark(SUDOKU_FC_OVARS_MIN_RESULT_FILE,
		&sudokuBenchmark, &sudokuArgs
	);
	printf("Started FC_OVARS_MIN benchmarking on Sudoku puzzles (%d).\n",
		sfcovminpid
	);

	pid_t pids[] = {npid, nfcovminpid, nfcpid, nfcovmaxpid, sfcovminpid,
		sfcpid, spid/*,sfcovmaxpid*/
	};
	const char* messages[] = {
		"Finished benchmarking (NQueens problems; %d).\n",
		"Finished FC benchmarking (NQueens problems; %d).\n",
		"Finished FC_OVARS_MIN benchmarking (NQueens problems; %d).\n",
		"Finished FC_OVARS_MAX benchmarking (NQueens problems; %d).\n",
		"Finished benchmarking (Sudoku puzzles; %d).\n",
		"Finished FC benchmarking (Sudoku puzzles; %d).\n",
		"Finished FC_OVARS_MIN benchmarking (Sudoku puzzles; %d).\n",
		/*"Finished FC_OVARS_MAX benchmarking (Sudoku puzzles; %d).\n",*/
	};
	for (int idx = 0; idx < 7; ++idx) {
		if (pids[idx] != -1 && waitpid(pids[idx], NULL, 0) == -1) {
			perror("waitpid");
			exitCode = EXIT_FAILURE;
		} else {
			printf(messages[idx], getpid());
		}
	}

  for (int i = 0; i < sudokuArgs.total_count; i++) {
	  free(sudokuArgs.sudokus[i]);
  }
  free(sudokuArgs.sudokus);

  return EXIT_SUCCESS;
}