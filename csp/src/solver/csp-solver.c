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

// PUBLIC
// Getters
bool csp_problem_is_consistent(const CSPProblem *csp,
	size_t *values, const void *data, size_t index, CSPChecklist *checklist
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
	size_t *values, const void *data, size_t index, CSPChecklist *checklist
) {
	assert(csp_initialised());

	// If all variables are assigned, the CSP is solved
	if(index == csp_problem_get_num_domains(csp)){
		return true;
	}

	// Try all values in the domain of the current variable
	for(size_t i = 0; i < csp_problem_get_domain(csp, index); i++) {
		// Assign the value to the variable
		values[index] = i;

		// Check if the assignment is consistent with the constraints
		if(csp_problem_is_consistent(csp, values, data, index, checklist)
			&& csp_problem_backtrack(csp, values, data, index+1, checklist)
		){
			return true;
		}
	}
	return false;
}

bool csp_problem_solve(const CSPProblem *csp, size_t *values, const void *data, CSPChecklist *checklist)
{
	assert(csp_initialised());

	// Start the backtracking algorithm
	return csp_problem_backtrack(csp, values, data, 0, checklist);
}
