/**
 * @file csp-solver-ovars.h
 * Library CSP variable heuristics
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#if !defined(_CSP_H_INSIDE) && !defined(CSP_COMPILATION)
#error "Only <csp/csp.h> can be included directly."
#endif

#include <stddef.h>

#include "core/csp-problem.h"
#include "solver/types-and-structs.h"

/**
 * Choose the next variable to assign in the CSP problem.
 * This function selects the variable with the smallest domain size
 * (Minimum Remaining Values heuristic).
 *
 * @param csp The CSP problem instance.
 * @param fv The structure tracking filled variables.
 * @param domains The array of domains for each variable.
 * @return The index of the chosen variable.
 */
extern size_t csp_problem_choose_min_domain(const CSPProblem *csp,
	const FilledVariables *fv, Domain **domains);

/**
 * Choose the next variable to assign in the CSP problem.
 * This function selects the variable with the largest domain size
 * (Maximum Remaining Values heuristic).
 *
 * @param csp
 * @param fv
 * @param domains
 * @return
 */
extern size_t csp_problem_choose_max_domain(const CSPProblem *csp,
	const FilledVariables *fv, Domain **domains);