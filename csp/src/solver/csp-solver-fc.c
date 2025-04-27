/**
* @file csp-solver-fc.c
 * Library CSP solving functions with forward checking
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "solver/csp-solver-fc.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#include "core/csp-constraint.h"
#include "core/csp-problem.h"

// PRIVATE
typedef struct {
	size_t amount;
	size_t *values;
}AvailableValues;

// PUBLIC

// Functions
bool csp_problem_backtrack_fc(const CSPProblem *csp,
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
			&& csp_problem_backtrack_fc(csp, values, data, index+1, checklist)
		){
			return true;
		}
	}
	return false;
}

bool csp_problem_solve_fc(const CSPProblem *csp, size_t *values, const void *data, CSPChecklist *checklist)
{
	assert(csp_initialised());

	// Start the backtracking algorithm
	return csp_problem_backtrack_fc(csp, values, data, 0, checklist);
}
