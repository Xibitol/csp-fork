/**
 * @file csp-solver.c
 * Defines functions to solve a CSP problem using backtracking algorithm.
 *
 * @author Ch. Demko
 * @date 2024
 */

#include "solver/csp-solver.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/csp-constraint.h"
#include "core/csp-problem.h"

// PRIVATE
static int backtrack_counter = 0;

void print_domains(Domain **domains, size_t num_domains) {
	for (size_t i = 0; i < num_domains; i++) {
		printf("Domain %zu: ", i);
		for (size_t j = 0; j < domains[i]->amount; j++) {
			printf("%zu ", domains[i]->values[j]);
		}
		printf("\n");
	}
	printf("\n");
}

void reduce_domains(const CSPProblem *csp, size_t* values, const void* data, Domain** domains, CSPDataChecklist dataChecklist) {
	if (dataChecklist == NULL) {
		return;
	}
	for (size_t i = 0; i < csp_problem_get_num_domains(csp); i++) {
		for (size_t j = 0; j < domains[i]->amount; /* no increment here */) {
		    values[i] = domains[i]->values[j];
		    if (!csp_problem_is_consistent(csp, values, data, i, dataChecklist)) {
		        // Remove the value from the domain
		        domains[i]->amount--;
		        for (size_t k = j; k < domains[i]->amount; k++) {
		            domains[i]->values[k] = domains[i]->values[k + 1];
		        }
		        // Do not increment j, as the next value is now at the same index
		    } else {
		        j++; // Increment only if no value was removed
		    }
		}
	}
}

// PUBLIC
// Getters
bool csp_problem_is_consistent(const CSPProblem *csp,
	size_t *values, const void *data, size_t index, CSPValueChecklist *checklist
){
	assert(csp_initialised());

	CSPConstraint **checks = malloc(sizeof(CSPConstraint*) * csp_problem_get_num_constraints(csp));
	if (checks == NULL) {
		perror("malloc");
		return false;
	}
	size_t amount = 0;

	// Get the list of checks to verify for the current index
	checklist(csp, checks, &amount, index);

	// if any check from the checklist fails, the CSP is not consistent
	for(size_t i = 0; i < amount; i++){
		if(!csp_constraint_get_check(checks[i])(checks[i], values, data)){
			free(checks);
			return false;
		}
	}
	free(checks);
	return true;
}

// Functions
bool csp_problem_backtrack(const CSPProblem *csp,
	size_t *values, const void *data, size_t index, CSPValueChecklist *checklist, Domain** domains
) {
	assert(csp_initialised());
	backtrack_counter++;

	// If all variables are assigned, the CSP is solved
	if(index == csp_problem_get_num_domains(csp)){
		return true;
	}

	// Try all values in the domain of the current variable
	for (size_t i = 0; i < domains[index]->amount; i++) {
		// Assign the value to the variable
		values[index] = domains[index]->values[i];

		// Check if the assignment is consistent with the constraints
		if(csp_problem_is_consistent(csp, values, data, index, checklist)
			&& csp_problem_backtrack(csp, values, data, index+1, checklist, domains)
		){
			return true;
		}
	}
	return false;
}

bool csp_problem_solve(const CSPProblem *csp, size_t *values, const void *data, CSPValueChecklist *checklist, CSPDataChecklist dataChecklist, size_t* benchmark)
{
	assert(csp_initialised());

	size_t num_domains = csp_problem_get_num_domains(csp);
	Domain *domains[num_domains];

	// Allocate memory for each domain
	for (size_t i = 0; i < num_domains; i++) {
		size_t domain_size = csp_problem_get_domain(csp, i);
		domains[i] = malloc(sizeof(Domain) + domain_size * sizeof(size_t));
		if (domains[i] == NULL) {
			perror("malloc");
			// Free previously allocated domains
			for (size_t j = 0; j < i; j++) {
				free(domains[j]);
			}
			return false;
		}
		domains[i]->amount = domain_size;
		for (size_t j = 0; j < domain_size; j++) {
			domains[i]->values[j] = j;
		}
	}

	reduce_domains(csp, values, data, domains, dataChecklist);

	// print_domains(domains, csp_problem_get_num_domains(csp)); //DEBUG

	bool result = csp_problem_backtrack(csp, values, data, 0, checklist, domains);

	// Free allocated memory
	for (size_t i = 0; i < num_domains; i++) {
		free(domains[i]);
	}

	if (benchmark!=NULL) {
		benchmark[0] = backtrack_counter;
	}
	// Reset the backtrack counter
	backtrack_counter = 0;

	// Start the backtracking algorithm
	return result;
}
