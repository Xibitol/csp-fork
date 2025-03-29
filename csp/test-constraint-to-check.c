#include <stdio.h>
#include <stdlib.h>

#include "./csp.h"
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

// Dummy check function
bool dummy_check(const CSPConstraint *constraint, const size_t *values, const void *data) {
  return true;
}

int main(void) {
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
    printf("Constraint to check 0: %d\n", csp_constraint_to_check(constraint, 0));
    assert(csp_constraint_to_check(constraint, 0) == false);
    printf("Constraint to check 1: %d\n", csp_constraint_to_check(constraint, 1));
    assert(csp_constraint_to_check(constraint, 1) == false);
    printf("Constraint to check 2: %d\n", csp_constraint_to_check(constraint, 2));
    assert(csp_constraint_to_check(constraint, 2) == true);
    // Destroy the constraint
    csp_constraint_destroy(constraint);
  }
  // Finish the library
  csp_finish();

  return EXIT_SUCCESS;
}