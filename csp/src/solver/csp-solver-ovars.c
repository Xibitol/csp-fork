/**
 * @file csp-solver-ovars.c
 * Library CSP variable heuristics
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "solver/csp-solver-ovars.h"

#include <assert.h>
#include <stddef.h>

#include "core/csp-problem.h"
#include "core/csp-lib.h"
#include "solver/types-and-structs.h"

size_t csp_problem_choose_min_domain(const CSPProblem *csp,
	const FilledVariables *fv, Domain **domains
){
	assert(csp_initialised());

	size_t index = 0;
	size_t min_domain_size = SIZE_MAX;

	for (size_t i = 0; i < csp_problem_get_num_domains(csp); i++) {
		if (!filled_variables_is_filled(fv, i)) {
			size_t domain_size = domains[i]->amount;
			if (domain_size < min_domain_size) {
				min_domain_size = domain_size;
				index = i;
				if (min_domain_size == 1) {	 // exit early
					break;
				}
			}
		}
	}

	return index;
}

size_t csp_problem_choose_max_domain(const CSPProblem *csp,
	const FilledVariables *fv, Domain **domains){
	assert(csp_initialised());

	size_t index = 0;
	size_t max_domain_size = 0;

	for (size_t i = 0; i < csp_problem_get_num_domains(csp); i++) {
		if (!filled_variables_is_filled(fv, i)) {
			size_t domain_size = domains[i]->amount;
			if (domain_size > max_domain_size) {
				max_domain_size = domain_size;
				index = i;
				// if (max_domain_size == ?) {	 // is there an exit early?
				// 	break;
				// }
			}
		}
	}

	return index;
}