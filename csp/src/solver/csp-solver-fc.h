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

#include <core/csp-problem.h>
#include <core/csp-constraint.h>
#include <solver/csp-solver.h>

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
extern bool csp_problem_solve_fc(const CSPProblem *csp,
	size_t *values, const void *data, CSPValueChecklist* checklist, size_t* benchmark);
