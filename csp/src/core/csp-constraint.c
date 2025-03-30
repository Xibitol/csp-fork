/**
 * @file csp-constraint.h
 * Defines types and operations for a `CSPConstraint`.
 *
 * @author Ch. Demko
 * @date 2024
 */

#include "csp-constraint.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "csp-lib.h"

#include "csp-constraint.inc.h"

// PUBLIC
// Constructors
CSPConstraint *csp_constraint_create(size_t arity, CSPChecker *check){
	assert(csp_initialised());
	assert(arity > 0);
	assert(check != NULL);
	assert(printf("Creating constraint with arity %lu\n", arity));

	// Allocate memory for the constraint
	CSPConstraint *constraint = malloc(
		sizeof(CSPConstraint) + arity * sizeof(size_t)
	);

	if(constraint != NULL){
		constraint->arity = arity;
		constraint->check = check;
		memset(constraint->variables, 0, arity * sizeof(size_t));
	}

	return constraint;
}

// Destructors
void csp_constraint_destroy(CSPConstraint *constraint){
	assert(csp_initialised());
	assert(printf("Destroying constraint with arity %lu\n", constraint->arity));

	free(constraint);
}

// Getters
size_t csp_constraint_get_arity(const CSPConstraint *constraint){
	assert(csp_initialised());

	return constraint->arity;
}
CSPChecker *csp_constraint_get_check(const CSPConstraint *constraint){
	assert(csp_initialised());

	return constraint->check;
}
size_t csp_constraint_get_variable(const CSPConstraint *constraint,
	size_t index
){
	assert(csp_initialised());
	assert(index < constraint->arity);

	return constraint->variables[index];
}

// Setters
void csp_constraint_set_variable(CSPConstraint *constraint,
	size_t index, size_t variable
){
	assert(csp_initialised());
	assert(index < constraint->arity);

	constraint->variables[index] = variable;
}