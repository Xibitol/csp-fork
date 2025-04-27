//
// Created by adrien on 27/04/25.
//

#include <stdbool.h>

#include "../btest/load-new-sudoku.h"
#include "../btest/solve-queens.h"
#include "../btest/solve-sudoku.h"

int main(void) {
	solve_queens(8, "test-fc-queens.txt", false);
	solve_sudoku(load_new_sudoku(45, 1)[0], "test-fc-sudoku.txt", false);
}