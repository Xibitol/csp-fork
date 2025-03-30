//
// Created by adrien on 28/03/25.
//

#include "./sudoku-basic-solve.h"

#include <assert.h>
#include <stddef.h>

#include "../core/csp-constraint.inc.h"
#include "../core/csp-problem.inc.h"

static size_t backtrack_counter = 0;

bool sudoku_basic_is_consistent(const CSPProblem *csp, const size_t *values, const void *data, size_t index) {
    assert(csp_initialised());
    // Check all constraints
    for (size_t i = 0; i < csp->num_constraints; i++) {
        CSPConstraint *constraint = csp->constraints[i];
        // Verify if the constraint has to be checked and check it
        if (/*csp_constraint_to_check(constraint, index) &&*/ !constraint->check(constraint, values, data)) {
            return false;
        }
    }
    return true;
}

bool sudoku_basic_solve(const CSPProblem *csp, size_t *values, const void *data) {
    assert(csp_initialised());
    reset_backtrack_counter();
    return sudoku_basic_backtrack(csp, values, data, 0);
}

bool sudoku_basic_backtrack(const CSPProblem *csp, size_t *values, const void *data, size_t index) {
    assert(csp_initialised());
    backtrack_counter++;
    // If all variables are assigned, the CSP is solved
    if (index == csp->num_domains) {
        return true;
    }
    // Try all values in the domain of the current variable
    for (size_t i = 0; i < csp->domains[index]; i++) {
        // Assign the value to the variable
        values[index] = i;
        // Check if the assignment is consistent with the constraints
        if (sudoku_basic_is_consistent(csp, values, data, index + 1) &&
            sudoku_basic_backtrack(csp, values, data, index + 1)) {
            return true;
            }
        // Reset the value to an invalid state
        values[index] = 9;
    }
    return false;
}

size_t get_backtrack_counter(void) {
    return backtrack_counter;
}

void reset_backtrack_counter(void) {
    backtrack_counter = 0;
}