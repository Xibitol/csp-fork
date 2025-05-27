/**
 * @file solve-queens.h
 * N_Queens problem solver using the CSP library to perform benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "csp.h"

extern int solve_queens(int queen_count, const char* resultFile,
	SolveType solve_type, bool silent
);
