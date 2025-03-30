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

// Dummy check function
bool dummy_check(const CSPConstraint *constraint,
	const size_t *values, const void *data
){
	return true;
}

int main(void){
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