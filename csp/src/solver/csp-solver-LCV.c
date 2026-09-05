/**
 * @file csp-solver-LCV.c
 * Library CSP value heuristics
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/csp-constraint.h"
#include "core/csp-lib.h"
#include "core/csp-problem.h"
#include "solver/csp-solver.h"
#include "solver/domains.h"
#include "solver/filled-variables.h"

int get_constraint_count(const CSPProblem *csp, size_t *values,
												 const void *data, size_t index, FilledVariables *fv,
												 CSPValueChecklist *checklist, Domain **domains) {
	assert(csp_initialised());

	int count = 0;

	CSPConstraint **variable_checks =
			malloc(sizeof(CSPConstraint *) * csp_problem_get_num_constraints(csp));
	if (variable_checks == NULL) {
		perror("malloc");
		return false;
	}

	for (size_t i = 0; i < filled_variables_get_size(fv); i++) {
		if (!filled_variables_is_filled(fv, i)) {
			size_t v_amount = 0;
			checklist(csp, variable_checks, &v_amount, i, fv);

			CSPConstraint *relevant_check = NULL;
			for (size_t check_i = 0; check_i < v_amount; check_i++) {
				size_t var0 = csp_constraint_get_variable(variable_checks[check_i], 0);
				size_t var1 = csp_constraint_get_variable(variable_checks[check_i], 1);
				if ((var0 == index && var1 == i) || (var1 == index && var0 == i)) {
					relevant_check = variable_checks[check_i];
					break;
				}
			}

			if (relevant_check == NULL) {
				continue;
			}

			for (size_t j = 0; j < domain_get_amount(domains[i]); j++) {
				values[i] = domain_get_value(domains[i], j);
				if (!csp_constraint_get_check(relevant_check)(relevant_check, values,
																											data)) {
					count++;
				}
			}
		}
	}

	free(variable_checks);
	return count;
}
