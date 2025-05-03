/**
 * @file csp-solver.h
 * Defines functions to solve a CSP problem using backtracking algorithm.
 *
 * @author Ch. Demko
 * @date 2024
 */

#pragma once

#if !defined (_CSP_H_INSIDE) && !defined (CSP_COMPILATION)
#error "Only <csp/csp.h> can be included directly."
#endif

#include <stdbool.h>
#include <stddef.h>

#include <core/csp-problem.h>
#include <core/csp-constraint.h>

/**
 * Get the list of constraints to verify for the current variable to know if the CSPProblem is consistent.
 * @note This function is used by #csp_problem_is_consistent.
 * @param csp The CSP problem.
 * @param checklist Array to store the list of constraints to verify.
 * @param amount Pointer to size_t to store the number of constraints to verify.
 * @param index The index of the current variable.
 */
typedef void CSPChecklist(const CSPProblem *csp, CSPConstraint** checklist, size_t* amount, size_t index);

/** Verify if the CSP problem is consistent at the specified index.
 * @param csp The CSP problem to verify.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @param index The index of the current variable.
 * @param checklist A pointer to function to get the list of necessary constraints for the current variable.
 * @return true if the CSP problem is consistent, false otherwise.
 * @pre The csp library is initialised.
 */
extern bool csp_problem_is_consistent(const CSPProblem *csp,
	size_t *values, const void *data, size_t index, CSPChecklist* checklist
);

// FUNCTIONS
/** Solve the CSP problem using backtracking.
 * @param csp The CSP problem to solve.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @param index The index of the variable to set.
 * @param checklist A pointer to function to get the list of necessary constraints for the current variable.
 * @return true if the CSP problem is solved, false otherwise.
 * @pre The csp library is initialised.
 * @post The values are assigned to the solution.
 */
extern bool csp_problem_backtrack(const CSPProblem *csp,
	size_t *values, const void *data, size_t index,
	CSPChecklist* checklist
);

/** Solve the CSP problem using backtracking.
 * @param csp The CSP problem to solve.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @param checklist A pointer to function to get the list of necessary constraints for the current variable.
 * @param benchmark pointer to Node counter for benchmarking, NULL if no benchmarking required
 * @return true if the CSP problem is solved, false otherwise.
 * @pre The csp library is initialised.
 * @post The values are assigned to the solution.
 */
extern bool csp_problem_solve(const CSPProblem *csp,
	size_t *values, const void *data, CSPChecklist* checklist, size_t* benchmark);
