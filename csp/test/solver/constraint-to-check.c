/**
 * @file constraint-to-check.h
 *
 * @author Ch. Demko
 * @date 2024
 */

#ifdef NDEBUG
	#undef NDEBUG
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "csp.h"
#include "util/unused.h"

// Dummy check function
bool test_solver_constraint_to_check__dummy_check(
	const CSPConstraint *UNUSED_VAR(constraint),
	const size_t *UNUSED_VAR(values),
	const void *UNUSED_VAR(data)
){
	return true;
}

int test_solver_constraint_to_check(void){
	CSPChecker *dummy_check = &test_solver_constraint_to_check__dummy_check;

	// Initialise the library
	csp_init();
	{
		// Create the constraint
		CSPConstraint *constraint = csp_constraint_create(2, dummy_check);

		// Check that the constraint is created correctly
		assert(constraint != NULL);

		// Set the variables of the constraint
		csp_constraint_set_variable(constraint, 0, 0);
		csp_constraint_set_variable(constraint, 1, 1);

		// Check the constraint
		printf("Constraint to check 0: %d\n",
			csp_constraint_to_check(constraint, 0)
		);
		assert(csp_constraint_to_check(constraint, 0) == false);

		printf("Constraint to check 1: %d\n",
			csp_constraint_to_check(constraint, 1)
		);
		assert(csp_constraint_to_check(constraint, 1) == false);

		printf("Constraint to check 2: %d\n",
			csp_constraint_to_check(constraint, 2)
		);
		assert(csp_constraint_to_check(constraint, 2) == true);

		// Destroy the constraint
		csp_constraint_destroy(constraint);
	}
	// Finish the library
	csp_finish();

	return EXIT_SUCCESS;
}