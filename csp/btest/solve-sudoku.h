/**
 * @file solve-sudoku.h
 * Sudoku solver using the CSP library to perform benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>

extern int sudoku_solve(size_t* sudoku, const char* resultFile, bool silent);