/**
* @file load-new-sudoku.h
 * Sudoku generator using a backtracking algorithm to generate test grids used in benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

extern size_t** load_new_sudoku(const int unknown_count, const int sudoku_count);
