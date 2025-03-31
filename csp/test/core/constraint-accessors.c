/**
 * @file constraint-accessors.h
 *
 * @author Ch. Demko
 * @date 2024
 */

#ifdef NDEBUG
	#undef NDEBUG
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "csp.h"
#include "util/unused.h"

// Dummy check function
bool test_core_constraint_accessors__dummy_check(
	const CSPConstraint *UNUSED_VAR(constraint),
	const size_t *UNUSED_VAR(values),
	const void *UNUSED_VAR(data)
){
	return true;
}

int test_core_constraint_accessors(void){
	CSPChecker *dummy_check = &test_core_constraint_accessors__dummy_check;

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

		assert(csp_constraint_get_variable(constraint, 0) == 0);
		assert(csp_constraint_get_variable(constraint, 1) == 1);

		// Destroy the constraint
		csp_constraint_destroy(constraint);
	}
	// Finish the library
	csp_finish();

	return EXIT_SUCCESS;
}