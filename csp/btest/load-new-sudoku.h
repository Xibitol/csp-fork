/**
 * @file load-new-sudoku.h
 * Sudoku generator using a backtracking algorithm to generate test grids used
 * in benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <stddef.h>

/**
 * Loads a new Sudoku puzzle with a specified number of unknown cells.
 * @param unknown_count amount of unknown cells in the Sudoku grid.
 * @param sudoku_count number of Sudoku grids to generate.
 * @return A pointer to an array of Sudoku grids, or NULL if an error occurred.
 */
extern size_t** load_new_sudoku(
	const int unknown_count, const int sudoku_count
);
