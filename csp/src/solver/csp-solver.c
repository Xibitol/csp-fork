/**
 * @file csp-solver.c
 * Defines functions to solve a CSP problem using backtracking algorithm.
 *
 * @author Ch. Demko
 * @date 2024
 */

#include "solver/csp-solver.h"

#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#include "core/csp-lib.h"
#include "core/csp-problem.h"
#include "core/csp-constraint.h"

// PUBLIC
// Getters
bool csp_constraint_to_check(const CSPConstraint *constraint, size_t index){
    assert(csp_initialised());

    for(size_t i = 0; i < csp_constraint_get_arity(constraint); i++){
		if(csp_constraint_get_variable(constraint, i) >= index){
			return false;
		}
    }

    return true;
}

bool csp_problem_is_consistent(const CSPProblem *csp,
	const size_t *values, const void *data, size_t index
){
	assert(csp_initialised());

	// Check all constraints
	for(size_t i = 0; i < csp_problem_get_num_constraints(csp); i++){
		CSPConstraint *constraint = csp_problem_get_constraint(csp, i);

		// Verify if the constraint has to be checked and check it
		if(csp_constraint_to_check(constraint, index)
			&& !csp_constraint_get_check(constraint)(constraint, values, data)
		){
			return false;
		}
	}
	return true;
}

// Functions
bool csp_problem_backtrack(const CSPProblem *csp,
	size_t *values, const void *data, size_t index, CSPConsistent *is_consistent
) {
	assert(csp_initialised());
	is_consistent = is_consistent == NULL ? &csp_problem_is_consistent : is_consistent;

	// If all variables are assigned, the CSP is solved
	if(index == csp_problem_get_num_domains(csp)){
		return true;
	}

	// Try all values in the domain of the current variable
	for(size_t i = 0; i < csp_problem_get_domain(csp, index); i++) {
		// Assign the value to the variable
		values[index] = i;

		// Check if the assignment is consistent with the constraints
		if(is_consistent(csp, values, data, index + 1)
			&& csp_problem_backtrack(csp, values, data, index + 1, is_consistent)
		){
			return true;
		}
	}
	return false;
}

bool csp_problem_solve(const CSPProblem *csp, size_t *values, const void *data, CSPConsistent *is_consistent)
{
	assert(csp_initialised());

	// Start the backtracking algorithm
	return csp_problem_backtrack(csp, values, data, 0, is_consistent);
}
