/**
 * @file csp-solver-ovars.c
 * Library CSP solving functions with forward checking + variable heuristics
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "solver/csp-solver-ovars.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/csp-constraint.h"
#include "core/csp-problem.h"
#include "solver/csp-solver-fc.h"
#include "solver/csp-solver.h"
#include "solver/types-and-structs.h"

static int backtrack_counter = 0;

size_t csp_problem_choose_variable(const CSPProblem *csp,
																	 const FilledVariables *fv,
																	 Domain **domains) {
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

bool csp_problem_backtrack_ovars(const CSPProblem *csp, size_t *values,
																 const void *data, FilledVariables *fv,
																 CSPValueChecklist *checklist, Domain **domains,
																 DomainChange *change_stack,
																 size_t *stack_top) {
	assert(csp_initialised());
	backtrack_counter++;

	// If all variables are assigned, the CSP is solved
	if (filled_variables_all_filled(fv)) {
		return true;
	}

	size_t stack_start = *stack_top;
	size_t index = csp_problem_choose_variable(csp, fv, domains);

	// Try all values in the domain of the current variable
	for (size_t i = 0; i < domains[index]->amount; i++) {
		// Assign the value to the variable
		values[index] = domains[index]->values[i];

		// print_values(values, index + 1); //DEBUG
		// print_domains_fc(domains, csp_problem_get_num_domains(csp)); //DEBUG

		// Check if the assignment is consistent with the constraints
		if (csp_problem_forward_check(csp, values, data, index, fv, checklist,
																	domains, change_stack, stack_top) &&
				csp_problem_backtrack_ovars(csp, values, data, fv, checklist, domains,
																		change_stack, stack_top)) {
			return true;
		}

		// Restore domains from the stack after backtracking
		domain_change_stack_restore(change_stack, stack_top, &stack_start, domains);
		// printf("backtracked\n"); //DEBUG
		// print_domains_fc(domains, csp_problem_get_num_domains(csp)); //DEBUG
	}
	return false;
}

// PUBLIC
// Functions
bool csp_problem_solve_ovars(const CSPProblem *csp, size_t *values,
														 const void *data, CSPValueChecklist *checklist,
														 CSPDataChecklist dataChecklist,
														 size_t *benchmark) {
	assert(csp_initialised());

	size_t num_domains = csp_problem_get_num_domains(csp);
	Domain *domains[num_domains];
	size_t stack_capacity = 0;

	FilledVariables *filled_vars = filled_variables_create(num_domains);

	// Allocate memory for each domain
	for (size_t i = 0; i < num_domains; i++) {
		size_t domain_size = csp_problem_get_domain(csp, i);
		stack_capacity += domain_size;
		domains[i] = domain_create(domain_size);
		if (domains[i] == NULL) {
			// Free previously allocated domains
			for (size_t j = 0; j < i; j++) {
				domain_destroy(domains[j]);
			}
			filled_variables_destroy(filled_vars);
			return false;
		}
	}

	// Initialize the change stack for forward checking
	DomainChange *change_stack = domain_change_stack_create(stack_capacity);
	if (change_stack == NULL) {
		for (size_t i = 0; i < num_domains; i++) {
			domain_destroy(domains[i]);
		}
		filled_variables_destroy(filled_vars);
		return false;
	}
	size_t stack_top = 0;

	reduce_domains(csp, values, data, domains, dataChecklist);

	// Start the backtracking algorithm
	bool result =
			csp_problem_backtrack_ovars(csp, values, data, filled_vars, checklist,
																	domains, change_stack, &stack_top);

	// Free allocated memory
	for (size_t i = 0; i < num_domains; i++) {
		domain_destroy(domains[i]);
	}
	domain_change_stack_destroy(change_stack);
	filled_variables_destroy(filled_vars);

	if (benchmark != NULL) {
		benchmark[0] = backtrack_counter;
	}

	// Reset the backtrack counter
	backtrack_counter = 0;

	return result;
}
