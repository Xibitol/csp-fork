/**
 * @file csp-problem.c
 * Defines types and operations for a `CSPProblem`.
 *
 * @author Ch. Demko
 * @date 2024
 */

#include "csp-problem.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "csp-lib.h"
#include "csp-constraint.h"

#include "csp-problem.inc.h"

// PUBLIC
// Constructors
CSPProblem *csp_problem_create(size_t num_domains, size_t num_constraints) {
	assert(csp_initialised());
	assert(num_domains > 0);
	assert(num_constraints > 0);
	assert(printf(
		"Creating CSP problem with %lu domains and %lu constraints\n",
		num_domains, num_constraints
	));

	// Allocate memory for the CSP problem
	CSPProblem *csp = malloc(sizeof(CSPProblem));

	if(csp != NULL){
		// Allocate memory for the domains of the CSP problem
		csp->domains = calloc(num_domains, sizeof(size_t));

		if(csp->domains != NULL){
			// Allocate memory for the contraints of the CSP problem
			csp->constraints = malloc(
				num_constraints * sizeof(CSPConstraint *)
			);

			if(csp->constraints != NULL){
				for(size_t i = 0; i < num_constraints; i++){
					csp->constraints[i] = NULL;
				}

				csp->num_domains = num_domains;
				csp->num_constraints = num_constraints;
			}else{
				free(csp->domains);
				free(csp);
				csp = NULL;
			}
		}else{
			free(csp);
			csp = NULL;
		}
	}else{
		free(csp);
		csp = NULL;
	}

	return csp;
}

// Destructors
void csp_problem_destroy(CSPProblem *csp) {
	assert(csp_initialised());
	assert(printf(
		"Destroying CSP problem with %lu domains and %lu constraints\n",
		csp->num_domains, csp->num_constraints
	));

	free(csp->constraints);
	free(csp->domains);
	free(csp);
}

// Getters
size_t csp_problem_get_num_constraints(const CSPProblem *csp){
	assert(csp_initialised());

	return csp->num_constraints;
}
CSPConstraint *csp_problem_get_constraint(const CSPProblem *csp, size_t index){
	assert(csp_initialised());
	assert(index < csp->num_constraints);

	return csp->constraints[index];
}
size_t csp_problem_get_num_domains(const CSPProblem *csp){
	assert(csp_initialised());

	return csp->num_domains;
}
size_t csp_problem_get_domain(const CSPProblem *csp, size_t index){
	assert(csp_initialised());
	assert(index < csp->num_domains);

	return csp->domains[index];
}

// Setters
void csp_problem_set_constraint(CSPProblem *csp,
	size_t index, CSPConstraint *constraint
){
	assert(csp_initialised());
	assert(index < csp->num_constraints);
	assert(constraint != NULL);

	csp->constraints[index] = constraint;
}
void csp_problem_set_domain(CSPProblem *csp, size_t index, size_t domain){
	assert(csp_initialised());
	assert(index < csp->num_domains);

	csp->domains[index] = domain;
}