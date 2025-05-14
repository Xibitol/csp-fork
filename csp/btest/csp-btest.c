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

#include <csp.h>
#include "util/unused.h"
#include "solve-queens.h"
#include "load-new-sudoku.h"
#include "solve-sudoku.h"

#define NQUEENS_RESULT_FILE "n_queens/_basic_benchmark.txt"
#define NQUEENS_OVARS_RESULT_FILE "n_queens/_ovars_benchmark.txt"
#define NQUEENS_FC_RESULT_FILE "n_queens/_fc_benchmark.txt"
#define NQUEENS_FC_OVARS_RESULT_FILE "n_queens/_fc_ovars_benchmark.txt"
#define SUDOKU_RESULT_FILE "sudoku/_basic_benchmark.txt"
#define SUDOKU_OVARS_RESULT_FILE "sudoku/_ovars_benchmark.txt"
#define SUDOKU_FC_RESULT_FILE "sudoku/_fc_benchmark.txt"
#define SUDOKU_FC_OVARS_RESULT_FILE "sudoku/_fc_ovars_benchmark.txt"

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
	NQueensArgs nqueensArgsB = {20, 0};
	pid_t npid = benchmark(NQUEENS_RESULT_FILE, nqueensBenchmark, &nqueensArgsB);
	printf("Started benchmarking on %d NQueens problems (%d).\n",
		nqueensArgsB.total_count, npid
	);

	NQueensArgs nqueensArgsOV = {20, OVARS};
	pid_t novpid = benchmark(NQUEENS_OVARS_RESULT_FILE, nqueensBenchmark, &nqueensArgsOV);
	printf("Started OVARS benchmarking on %d NQueens problems (%d).\n",
		nqueensArgsOV.total_count, novpid
	);

	NQueensArgs nqueensArgsFC = {20, FC};
  pid_t nfcpid = benchmark(NQUEENS_FC_RESULT_FILE, nqueensBenchmark, &nqueensArgsFC);
  printf("Started FC benchmarking on %d NQueens problems (%d).\n",
	  nqueensArgsFC.total_count, nfcpid
  );

	NQueensArgs nqueensArgsFCOV = {20, FC | OVARS};
	pid_t nfcovpid = benchmark(NQUEENS_FC_OVARS_RESULT_FILE, nqueensBenchmark, &nqueensArgsFCOV);
	printf("Started FC_OVARS benchmarking on %d NQueens problems (%d).\n",
	  nqueensArgsFCOV.total_count, nfcovpid
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

	sudokuArgs.solve_type = 0;
  pid_t spid = benchmark(SUDOKU_RESULT_FILE, &sudokuBenchmark, &sudokuArgs);
  printf("Started benchmarking on Sudoku puzzles (%d).\n", spid);

	sudokuArgs.solve_type = OVARS;
	pid_t sovpid = benchmark(SUDOKU_OVARS_RESULT_FILE, &sudokuBenchmark, &sudokuArgs);
	printf("Started OVARS benchmarking on Sudoku puzzles (%d).\n", sovpid);

	sudokuArgs.solve_type = FC;
  pid_t sfcpid = benchmark(SUDOKU_FC_RESULT_FILE, &sudokuBenchmark, &sudokuArgs);
  printf("Started FC benchmarking on Sudoku puzzles (%d).\n", sfcpid);

	sudokuArgs.solve_type = FC | OVARS;
	pid_t sfcovpid = benchmark(SUDOKU_FC_OVARS_RESULT_FILE, &sudokuBenchmark, &sudokuArgs);
	printf("Started FC_OVARS benchmarking on Sudoku puzzles (%d).\n", sfcovpid);

	pid_t pids[] = {npid, novpid, nfcpid, nfcovpid, sfcovpid, sfcpid, spid, sovpid};
	const char* messages[] = {
		"Finished benchmarking (NQueens problems; %d).\n",
		"Finished OVARS benchmarking (NQueens problems; %d).\n",
		"Finished FC benchmarking (NQueens problems; %d).\n",
		"Finished FC_OVARS benchmarking (NQueens problems; %d).\n",
		"Finished FC_OVARS benchmarking (Sudoku puzzles; %d).\n",
		"Finished FC benchmarking (Sudoku puzzles; %d).\n",
		"Finished benchmarking (Sudoku puzzles; %d).\n",
		"Finished OVARS benchmarking (Sudoku puzzles; %d).\n"
	};
	for (int idx = 0; idx < 8; ++idx) {
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