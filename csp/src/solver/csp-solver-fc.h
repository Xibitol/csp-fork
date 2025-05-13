/**
* @file csp-solver-fc.h
 * Library CSP solving functions with forward checking
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#if !defined (_CSP_H_INSIDE) && !defined (CSP_COMPILATION)
#error "Only <csp/csp.h> can be included directly."
#endif

#include <stdbool.h>
#include <stddef.h>

#include "core/csp-problem.h"
#include "solver/types-and-structs.h"

/**
 * Forward check the CSP problem. Updates the domains of the variables
 * @param csp The CSP problem to check.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @param index The index of the current variable.
 * @param fv The filled variables structure to track filled variables.
 * @param checklist A pointer to function to get the list of necessary constraints for the current variable.
 * @param domains The domains of the variables.
 * @param change_stack The stack of changes made during forward checking.
 * @param stack_top The top of the change stack.
 * @return true if the CSP problem is consistent, false otherwise.
 * @pre The csp library is initialised.
 */
extern bool csp_problem_forward_check(const CSPProblem *csp, size_t *values,
	const void *data, size_t index, FilledVariables* fv, CSPValueChecklist *checklist, Domain **domains,
	DomainChange *change_stack, size_t *stack_top
);

/** Solve the CSP problem using backtracking.
 * @param csp The CSP problem to solve.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @param checklist A pointer to function to get the list of necessary constraints for the current variable.
 * @param dataChecklist A pointer to function to get the list of necessary data constraints for the current variable.
 * @param benchmark pointer to Node counter for benchmarking, NULL if no benchmarking required
 * @return true if the CSP problem is solved, false otherwise.
 * @pre The csp library is initialised.
 * @post The values are assigned to the solution.
 */
extern bool csp_problem_solve_fc(const CSPProblem *csp,
	size_t *values, const void *data, CSPValueChecklist* checklist, CSPDataChecklist dataChecklist, size_t* benchmark);
