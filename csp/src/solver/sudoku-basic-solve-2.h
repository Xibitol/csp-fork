//
// Created by adrien on 29/03/25.
//

#ifndef SUDOKU_BASIC_SOLVE_2_H
#define SUDOKU_BASIC_SOLVE_2_H

#endif //SUDOKU_BASIC_SOLVE_2_H


#include <stdbool.h>
#include <stddef.h>

#include "../csp.h"

bool sudoku_basic_is_consistent(const CSPProblem *csp, const size_t *values, const void *data, size_t index);

bool sudoku_basic_solve(const CSPProblem *csp, size_t *values, const void *data);

bool sudoku_basic_backtrack(const CSPProblem *csp, size_t *values, const void *data, size_t index);

/**
 * @brief Get the counter that keeps track of the number of calls of the csp_problem_backtrack method.
 */
extern size_t get_backtrack_counter(void);

/**
 * @brief Resets the counter that keeps track of the number of calls of the csp_problem_backtrack method.
 */
extern void reset_backtrack_counter(void);