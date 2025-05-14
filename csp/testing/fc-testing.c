//
// Created by adrien on 27/04/25.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../btest/load-new-sudoku.h"
#include "../btest/solve-queens.h"
#include "../btest/solve-sudoku.h"

void save_puzzle_as_bytes(const size_t* puzzle, const char* filename) {
	FILE* file = fopen(filename, "wb");
	if (file == NULL) {
		perror("fopen");
		return;
	}

	fwrite(puzzle, sizeof(size_t), 81, file);
	fclose(file);
}

size_t* load_puzzle_from_bytes(const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (file == NULL) {
		perror("fopen");
		return NULL;
	}

	size_t* puzzle = malloc(81 * sizeof(size_t));
	if (puzzle == NULL) {
		perror("malloc");
		fclose(file);
		return NULL;
	}

	fread(puzzle, sizeof(size_t), 81, file);
	fclose(file);
	return puzzle;
}

int main(void) {
	SolveType basic = BASIC;
	SolveType fc = FC;
	SolveType fc_ovars = FC_OVARS;
  solve_queens(10, "test-fc-queens.txt", basic, false);
	solve_queens(10, "test-fc-queens.txt", fc, false);
	solve_queens(10, "test-fc-queens.txt", fc_ovars, false);

  size_t** sudoku = load_new_sudoku(50, 1);

  // Save the puzzle as bytes
  save_puzzle_as_bytes(sudoku[0], "current-puzzle.bin");

  // Load the puzzle back
  size_t* loaded_puzzle = load_puzzle_from_bytes("current-puzzle.bin");

  // Use the loaded puzzle (e.g., solve it)
  solve_sudoku(loaded_puzzle, "test-fc-sudoku.txt", basic, false);
  solve_sudoku(loaded_puzzle, "test-fc-sudoku.txt", fc, false);
	solve_sudoku(loaded_puzzle, "test-fc-sudoku.txt", fc_ovars, false);

  free(sudoku[0]);
  free(sudoku);
  free(loaded_puzzle);
  return 0;
}