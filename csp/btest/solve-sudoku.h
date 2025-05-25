/**
 * @file solve-sudoku.h
 * Sudoku solver using the CSP library to perform benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <stddef.h>

#include "csp.h"

extern int solve_sudoku(size_t* starter_grid, const char* resultFile,
	SolveType solve_type, bool silent
);
