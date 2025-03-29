#include <stdlib.h>

#include "./csp.h"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

bool different(const CSPConstraint *constraint, const size_t *values, const void *data) {
  size_t v0 = csp_constraint_get_variable(constraint, 0);
  size_t v1 = csp_constraint_get_variable(constraint, 1);
  return values[v0] != values[v1];
}

int main(void) {
  // Initialise the library
  csp_init();
  {
    // Create the values for the problem
    size_t values[] = {0, 0};
    // Create the problem
    CSPProblem *problem = csp_problem_create(2, 1);
    // Check the problem is created correctly
    assert(problem != NULL);
    // Create the constraint
    csp_problem_set_constraint(problem, 0, csp_constraint_create(2, (CSPChecker *)different));
    csp_constraint_set_variable(csp_problem_get_constraint(problem, 0), 0, 0);
    csp_constraint_set_variable(csp_problem_get_constraint(problem, 0), 1, 1);
    // Set the domains of the problem
    csp_problem_set_domain(problem, 0, 2);
    csp_problem_set_domain(problem, 1, 2);
    // Check the consistency of the problem
    assert(csp_problem_is_consistent(problem, values, NULL, 0));
    // Backtrack the problem
    csp_problem_backtrack(problem, values, NULL, 0);
    // Check the consistency of the problem
    assert(csp_problem_is_consistent(problem, values, NULL, 2));
    // Verify that the problem has been solved
    assert(values[0] != values[1]);
    assert(values[0] == 0 || values[0] == 1);
    assert(values[1] == 0 || values[1] == 1);
    // Destroy the constraint
    csp_constraint_destroy(csp_problem_get_constraint(problem, 0));
    // Destroy the problem
    csp_problem_destroy(problem);
  }
  // Finish the library
  csp_finish();

  return EXIT_SUCCESS;
}