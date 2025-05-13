/**
* @file csp-solver-ovars.c
 * Library CSP solving functions with forward checking + variable heuristics
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

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
#include "solver/csp-solver-ovars.h"
#include "solver/types-and-structs.h"

// PRIVATE
// void print_domains_fc(Domain **domains, size_t num_domains) {
// 	for (size_t i = 0; i < num_domains; i++) {
// 		printf("Domain %zu: ", i);
// 		for (size_t j = 0; j < domains[i]->amount; j++) {
// 			printf("%zu ", domains[i]->values[j]);
// 		}
// 		printf("\n");
// 	}
// 	printf("\n");
// }
//
// void print_values(size_t* values, size_t num_domains) {
// 	for (size_t i = 0; i < num_domains; i++) {
// 		printf("%zu ", values[i]);
// 	}
// 	printf("\n");
// }

static int backtrack_counter = 0;

bool csp_problem_backtrack_ovars(const CSPProblem *csp, size_t *values,
																 const void *data, size_t index,
																 CSPValueChecklist *checklist, Domain **domains,
																 DomainChange *change_stack,
																 size_t *stack_top) {
	assert(csp_initialised());
	backtrack_counter++;

	// If all variables are assigned, the CSP is solved
	if (index == csp_problem_get_num_domains(csp)) {
		return true;
	}

	size_t stack_start = *stack_top;
	// Track the stack size at the start of the call

	// Try all values in the domain of the current variable
	for (size_t i = 0; i < domains[index]->amount; i++) {
		// Assign the value to the variable
		values[index] = domains[index]->values[i];

		// print_values(values, index + 1); //DEBUG
		// print_domains_fc(domains, csp_problem_get_num_domains(csp)); //DEBUG

		// Check if the assignment is consistent with the constraints
		if (csp_problem_is_consistent(csp, values, data, index, checklist) &&
				csp_problem_forward_check(csp, values, data, index, checklist, domains,
																	change_stack, stack_top, *stack_top) &&
				csp_problem_backtrack_ovars(csp, values, data, index + 1, checklist,
																		domains, change_stack, stack_top)) {
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
	size_t stack_capacity = 0; // Adjust as needed

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
			return false;
		}
	}

	// Initialize the change stack for forward checking
	DomainChange *change_stack = domain_change_stack_create(stack_capacity);
	if (change_stack == NULL) {
		for (size_t i = 0; i < num_domains; i++) {
			domain_destroy(domains[i]);
		}
		return false;
	}
	size_t stack_top = 0;

	reduce_domains(csp, values, data, domains, dataChecklist);

	// Start the backtracking algorithm
	bool result = csp_problem_backtrack_ovars(csp, values, data, 0, checklist,
																						domains, change_stack, &stack_top);

	// Free allocated memory
	for (size_t i = 0; i < num_domains; i++) {
		domain_destroy(domains[i]);
	}
	domain_change_stack_destroy(change_stack);

	if (benchmark != NULL) {
		benchmark[0] = backtrack_counter;
	}

	// Reset the backtrack counter
	backtrack_counter = 0;

	return result;
}
