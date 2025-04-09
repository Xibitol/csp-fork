/**
 * @file csp-solver.h
 * Defines functions to solve a CSP problem using backtracking algorithm.
 *
 * @author Ch. Demko
 * @date 2024
 */

#pragma once

#if !defined (_CSP_H_INSIDE)
#error "Only <csp/csp.h> can be included directly."
#endif

#include <stddef.h>

// GETTERS
/**
 * @brief Verify if the contraint can be checked.
 * @param constraint The constraint to verify.
 * @param index The index of the last variable.
 * @return true if the constraint can be checked, false otherwise.
 * @pre The csp library is initialised.
 * @pre constraint != NULL
 */
extern bool csp_constraint_to_check(const CSPConstraint *constraint,
	size_t index
);

/**
 * @brief Verify if the CSP problem is consistent at the specified index.
 * @param csp The CSP problem to verify.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @param index The index of the current variable.
 * @return true if the CSP problem is consistent, false otherwise.
 * @pre The csp library is initialised.
 */
extern bool csp_problem_is_consistent(const CSPProblem *csp,
	const size_t *values, const void *data, size_t index
);

// FUNCTIONS
/**
 * @brief Solve the CSP problem using backtracking.
 * @param csp The CSP problem to solve.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @param index The index of the variable to set.
 * @return true if the CSP problem is solved, false otherwise.
 * @pre The csp library is initialised.
 * @post The values are assigned to the solution.
 */
extern bool csp_problem_backtrack(const CSPProblem *csp,
	size_t *values, const void *data, size_t index
);

/**
 * @brief Solve the CSP problem using backtracking.
 * @param csp The CSP problem to solve.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @return true if the CSP problem is solved, false otherwise.
 * @pre The csp library is initialised.
 * @post The values are assigned to the solution.
 */
extern bool csp_problem_solve(const CSPProblem *csp,
	size_t *values, const void *data
);