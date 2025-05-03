/**
* @file csp-solver-fc.c
 * Library CSP solving functions with forward checking
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "solver/csp-solver-fc.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/csp-constraint.h"
#include "core/csp-problem.h"

// PRIVATE
typedef struct {
	size_t amount;
	size_t values[];
}Domain;

bool csp_problem_forward_check(const CSPProblem *csp, size_t *values,
  const void *data, size_t index, CSPChecklist *checklist, Domain **domains
){
	CSPConstraint** variable_checks = malloc(sizeof(CSPConstraint*) * csp_problem_get_num_constraints(csp));
	if (variable_checks == NULL) {
		perror("malloc");
		return false;
	}

  for (size_t i = index + 1; i < csp_problem_get_num_domains(csp); i++) { // For all next variables

    size_t v_amount = 0;
    checklist(csp, variable_checks, &v_amount, i); // Get relevant constraints for variable `i`

    CSPConstraint *relevant_check = NULL;
    for (size_t check_i = 0; check_i < v_amount; check_i++) {
      if (csp_constraint_get_variable(variable_checks[check_i], 0) == index &&
          csp_constraint_get_variable(variable_checks[check_i], 1) == i) {
          relevant_check = variable_checks[check_i];
          break;
      }
    }

    if (relevant_check == NULL) { // No relevant constraint found
      continue;
    }

    for (size_t j = 0; j < domains[i]->amount; j++) {
	    // Test all remaining domain values
    	values[i] = domains[i]->values[j];

    	if (!csp_constraint_get_check(relevant_check)(relevant_check, values, data)) {
    		// Remove the value from the domain
				domains[i]->amount--;
    		for (int k = j; k < domains[i]->amount; k++) {
    			domains[i]->values[k] = domains[i]->values[k + 1];
    		}
				j--;
			}
    }
  	if (domains[i]->amount == 0) {
			// No values left in the domain, backtrack
			free(variable_checks);
			return false;
		}
  }
	free(variable_checks);
  return true;
}

bool csp_problem_backtrack_fc(const CSPProblem *csp,
	size_t *values, const void *data, size_t index, CSPChecklist *checklist,
	Domain **domains
) {
	assert(csp_initialised());

	// If all variables are assigned, the CSP is solved
	if(index == csp_problem_get_num_domains(csp)){
		return true;
	}

	// Try all values in the domain of the current variable
	for(size_t i = 0; i < domains[index]->amount; i++) {
		// Assign the value to the variable
		values[index] = domains[index]->values[i];

		// Check if the assignment is consistent with the constraints
		if(csp_problem_is_consistent(csp, values, data, index, checklist)
			&& csp_problem_forward_check(csp, values, data, index, checklist, domains)
			&& csp_problem_backtrack_fc(csp, values, data, index+1, checklist, domains)
		){
			return true;
		}
	}
	return false;
}

// PUBLIC
// Functions
bool csp_problem_solve_fc(const CSPProblem *csp, size_t *values, const void *data, CSPChecklist *checklist)
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

	// Start the backtracking algorithm
	bool result = csp_problem_backtrack_fc(csp, values, data, 0, checklist, domains);

	// Free allocated memory
	for (size_t i = 0; i < num_domains; i++) {
		free(domains[i]);
	}

	return result;
}
