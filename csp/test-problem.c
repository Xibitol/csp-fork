#include <stdlib.h>

#include "./csp.h"
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

int main(void) {
  // Initialise the library
  csp_init();
  {
    // Verify that the csp library has been initialised
    assert(csp_initialised());
    // Create the problem
    CSPProblem *problem = csp_problem_create(8, 28);
    // Check the problem is created correctly
    assert(problem != NULL);
    assert(csp_problem_get_num_domains(problem) == 8);
    assert(csp_problem_get_num_constraints(problem) == 28);
    for (size_t index = 0; index < 8; index++) {
      assert(csp_problem_get_domain(problem, index) == 0);
    }
    for (size_t index = 0; index < 28; index++) {
      assert(csp_problem_get_constraint(problem, index) == NULL);
    }
    // Destroy the problem
    csp_problem_destroy(problem);
  }
  // Finish the library
  csp_finish();
  // Verify that the csp library has been finished
  assert(!csp_initialised());

  return EXIT_SUCCESS;
}