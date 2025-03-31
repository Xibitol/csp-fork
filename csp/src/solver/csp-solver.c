/**
* @file csp-solver.c
 * Defines functions to solve a CSP problem using backtracking algorithm.
 *
 * @author Ch. Demko
 * @date 2024
 */

#include <assert.h>
#include <stddef.h>

#include "../csp.h"

static size_t backtrack_counter = 0;

bool csp_constraint_to_check(const CSPConstraint *constraint, size_t index) {
    assert(csp_initialised());
    for (size_t i = 0; i < csp_constraint_get_arity(constraint); i++) { // arity = 2
        // if one of the two queens is in a row after the row of the current queen, we don't check the constraint
        // because we haven't set a valid row for the queens beyond 'index' yet (if we have on a previous backtrack, it
        // wouldn't be valid anymore)
        if (csp_constraint_get_variable(constraint, i) >= index) {
            return false;
        }
    }
    return true;
}

bool csp_problem_is_consistent(const CSPProblem *csp, const size_t *values, const void *data, const size_t index, const bool sudoku) {
    assert(csp_initialised());
    if (sudoku) {
        CSPConstraint *constraint = csp_problem_get_constraint(csp, index - 1);
        return csp_constraint_get_check(constraint)(constraint, values, data);
    }

    // Check all constraints
    for (size_t i = 0; i < csp_problem_get_num_constraints(csp); i++) {
        CSPConstraint *constraint = csp_problem_get_constraint(csp, i);
        // Verify if the constraint has to be checked and check it
        if (csp_constraint_to_check(constraint, index) &&
            !csp_constraint_get_check(constraint)(constraint, values, data)) {
            return false;
        }
    }
    return true;
}

bool csp_problem_backtrack(const CSPProblem *csp, size_t *values, const void *data, const size_t index, const bool sudoku) {
    assert(csp_initialised());
    backtrack_counter++;
    // If all variables are assigned, the CSP is solved
    if (index == csp_problem_get_num_domains(csp)) {
        return true;
    }
    // Try all values in the domain of the current variable
    for (size_t i = 0; i < csp_problem_get_domain(csp, index); i++) {
        // Assign the value to the variable
        values[index] = i;
        // Check if the assignment is consistent with the constraints
        if (csp_problem_is_consistent(csp, values, data, index + 1, sudoku) &&
            csp_problem_backtrack(csp, values, data, index + 1, sudoku)) {
            return true;
        }
        if (sudoku) {
            // Reset the value to an invalid state
            values[index] = 9;
        }
    }
    return false;
}

bool csp_problem_solve(const CSPProblem *csp, size_t *values, const void *data, const bool sudoku) {
    assert(csp_initialised());
    return csp_problem_backtrack(csp, values, data, 0, sudoku);
}

size_t get_backtrack_counter(void) {
    return backtrack_counter;
}

void reset_backtrack_counter(void) {
    backtrack_counter = 0;
}