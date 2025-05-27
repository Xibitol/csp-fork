/**
 * @file csp-solver-fc.h
 * Library CSP forward checking
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
 * @param checklist A pointer to function to get the list of necessary
 * constraints for the current variable.
 * @param domains The domains of the variables.
 * @param change_stack The stack of changes made during forward checking.
 * @param stack_top The top of the change stack.
 * @return true if the CSP problem is consistent, false otherwise.
 * @pre The csp library is initialised.
 */
extern bool csp_problem_forward_check(const CSPProblem *csp, size_t *values,
	const void *data, size_t index,
	FilledVariables* fv, CSPValueChecklist *checklist, Domain **domains,
	DomainChange *change_stack, size_t *stack_top
);
