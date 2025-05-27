/**
 * @file csp-solver.c
 * Defines functions to solve a CSP problem using backtracking algorithm.
 *
 * @author Ch. Demko
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "solver/csp-solver.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/csp-constraint.h"
#include "core/csp-problem.h"
#include "solver/csp-solver-fc.h"
#include "solver/csp-solver-ovars.h"
#include "solver/types-and-structs.h"

int backtrack_counter = 0;

void reduce_domains(const CSPProblem *csp, size_t *values, const void *data,
	Domain **domains, CSPDataChecklist dataChecklist
){
	if (dataChecklist == NULL) {
		return;
	}
	for (size_t i = 0; i < csp_problem_get_num_domains(csp); i++) {
		for (size_t j = 0; j < domains[i]->amount; /* no increment here */) {

			CSPConstraint *checks[csp_problem_get_num_constraints(csp)];
			size_t amount = 0;

			dataChecklist(csp, checks, &amount, i);

			values[i] = domains[i]->values[j];
			bool consistent = true;
			for (size_t k = 0; k < amount; k++) {
				if (!csp_constraint_get_check(checks[k])(
					checks[k], values, data
				)){
					consistent = false;
					break;
				}
			}
			if (!consistent) {
				// Remove the value from the domain
				domains[i]->amount--;
				for (size_t k = j; k < domains[i]->amount; k++) {
					domains[i]->values[k] = domains[i]->values[k + 1];
				}
				// Do not increment j, as the next value is now at the same
				// index
			} else {
				j++;	// Increment only if no value was removed
			}
		}
	}
}

// PUBLIC
// Getters
bool csp_problem_is_consistent(const CSPProblem *csp, const size_t *values,
	const void *data, size_t index, FilledVariables *fv,
	CSPValueChecklist *checklist
){
	assert(csp_initialised());

	CSPConstraint *checks[csp_problem_get_num_constraints(csp)];
	size_t amount = 0;

	// Get the list of checks to verify for the current index
	checklist(csp, checks, &amount, index, fv);

	// if any check from the checklist fails, the CSP is not consistent
	for (size_t i = 0; i < amount; i++) {
		if (!csp_constraint_get_check(checks[i])(checks[i], values, data)) {
			return false;
		}
	}
	return true;
}

// Functions
bool csp_problem_backtrack(const CSPProblem *csp, size_t *values,
	const void *data, FilledVariables *fv, SolveType solve_type,
	CSPValueChecklist *checklist, Domain **domains, DomainChange *change_stack,
	size_t *stack_top
){
	assert(csp_initialised());
	backtrack_counter++;

	// If all variables are assigned, the CSP is solved
	if (filled_variables_all_filled(fv)) {
		return true;
	}

	size_t index;
	size_t stack_start;
	if (solve_type & FC) {
		stack_start = *stack_top;
	}

	if (solve_type & OVARS_MIN) {
		index = csp_problem_choose_min_domain(csp, fv, domains);
	} else if (solve_type & OVARS_MAX) {
		index = csp_problem_choose_max_domain(csp, fv, domains);
	} else {
		index = filled_variables_next_unfilled(fv, 0);
	}

	filled_variables_mark_filled(fv, index);

	// Try all values in the domain of the current variable
	for (size_t i = 0; i < domains[index]->amount; i++) {
		// Assign the value to the variable
		values[index] = domains[index]->values[i];

		// print_domains(domains, csp_problem_get_num_domains(csp)); //DEBUG

		bool result;
		if (solve_type & FC) {
			result = csp_problem_forward_check(csp, values, data, index, fv,
					checklist, domains, change_stack, stack_top
				) && csp_problem_backtrack(csp, values, data, fv, solve_type,
					checklist, domains, change_stack, stack_top
			);
		} else {
			result = csp_problem_is_consistent(csp, values, data, index, fv,
					checklist
				) && csp_problem_backtrack(csp, values, data, fv, solve_type,
					checklist, domains, NULL, NULL
			);
		}
		// Check if the assignment is consistent with the constraints
		if (result) {
			return true;
		}
		if (solve_type & FC) {
			// Restore domains from the stack after backtracking
			domain_change_stack_restore(change_stack, stack_top, &stack_start,
				domains
			);
		}
	}
	filled_variables_mark_unfilled(fv, index);
	return false;
}

bool csp_problem_solve(const CSPProblem *csp, size_t *values, const void *data,
	SolveType solve_type, CSPValueChecklist *checklist,
	CSPDataChecklist dataChecklist, size_t *benchmark
){
	assert(csp_initialised());

	size_t num_domains = csp_problem_get_num_domains(csp);
	FilledVariables *fv = filled_variables_create(num_domains);
	if (fv == NULL) {
		return false;
	}
	size_t stack_capacity = 0; // for FC

	Domain *domains[num_domains];
	// Allocate memory for each domain
	for (size_t i = 0; i < num_domains; i++) {
		size_t domain_size = csp_problem_get_domain(csp, i);
		stack_capacity += domain_size; // for FC
		domains[i] = domain_create(domain_size);
		if (domains[i] == NULL) {
			// Free previously allocated domains
			for (size_t j = 0; j < i; j++) {
				domain_destroy(domains[j]);
			}
			filled_variables_destroy(fv);
			return false;
		}
	}

	reduce_domains(csp, values, data, domains, dataChecklist);

	bool result;
	if (solve_type & FC) {
		// Initialize the change stack
		DomainChange *change_stack = domain_change_stack_create(stack_capacity);
		if (change_stack == NULL) {
			for (size_t i = 0; i < num_domains; i++) {
				domain_destroy(domains[i]);
			}
			filled_variables_destroy(fv);
			return false;
		}
		size_t stack_top = 0;

		result = csp_problem_backtrack(csp, values, data, fv, solve_type,
			checklist, domains, change_stack, &stack_top
		);

		domain_change_stack_destroy(change_stack);
	} else {
		result = csp_problem_backtrack(csp, values, data, fv, solve_type,
			checklist, domains, NULL, NULL
		);
	}

	// Free allocated memory
	for (size_t i = 0; i < num_domains; i++) {
		domain_destroy(domains[i]);
	}

	if (benchmark != NULL) {
		benchmark[0] = backtrack_counter;
	}
	// Reset the backtrack counter
	backtrack_counter = 0;

	filled_variables_destroy(fv);

	// Start the backtracking algorithm
	return result;
}
