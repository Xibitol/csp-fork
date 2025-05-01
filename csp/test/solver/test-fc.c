/**
 * @file test-fc.c
 * Minimal tests for CSP solver with forward checking
 * 
 * @author vgegaden
 * @date 2024
 */

 #include <stdio.h>
 #include <assert.h>
 #include <stdbool.h>
 
 #include "csp.h"
 #include "solver/csp-solver-fc.h"
 
 // Test constraint: X0 != X1
 static bool test_constraint_diff(const CSPConstraint *constraint, 
                                const size_t *values, 
                                const void *data) {
     (void)data; // Unused parameter
     
     size_t x0 = values[csp_constraint_get_variable(constraint, 0)];
     size_t x1 = values[csp_constraint_get_variable(constraint, 1)];
     
     return x0 != x1;
 }
 
 // Simple consistency function for testing
 static bool test_consistent(const CSPProblem *csp, 
                           const size_t *values, 
                           const void *data, 
                           size_t index,
                           bool **domains) {
     (void)domains; // Not used in basic test
     (void)data;    // Not used in basic test
     
     // Just use the normal consistency check
     for (size_t i = 0; i < csp_problem_get_num_constraints(csp); i++) {
         CSPConstraint *constraint = csp_problem_get_constraint(csp, i);
         if (!csp_constraint_get_check(constraint)(constraint, values, NULL)) {
             return false;
         }
     }
     return true;
 }
 
 int main_(void) {
     // Initialize CSP library
     assert(csp_init());
     
     printf("=== Testing CSP Solver with Forward Checking ===\n");
     
     // Create a simple CSP problem with 2 variables, domain size 2, and 1 constraint
     CSPProblem *problem = csp_problem_create(2, 1);
     assert(problem != NULL);
     
     // Set domains (both variables can be 0 or 1)
     csp_problem_set_domain(problem, 0, 2); // Domain size 2 for variable 0
     csp_problem_set_domain(problem, 1, 2); // Domain size 2 for variable 1
     
     // Create constraint: X0 != X1
     CSPConstraint *constraint = csp_constraint_create(2, test_constraint_diff);
     assert(constraint != NULL);
     
     // Set constraint variables
     csp_constraint_set_variable(constraint, 0, 0); // X0
     csp_constraint_set_variable(constraint, 1, 1); // X1
     
     // Add constraint to problem
     csp_problem_set_constraint(problem, 0, constraint);
     
     // Array to store solution
     size_t solution[2] = {0};
     
     // Solve with forward checking
     bool solved = csp_problem_solve_fc(problem, solution, NULL, test_consistent);
     
     // Verify solution
     printf("Solution found: %s\n", solved ? "true" : "false");
     if (solved) {
         printf("X0 = %lu, X1 = %lu\n", solution[0], solution[1]);
         assert(solution[0] != solution[1]); // Check constraint
     } else {
         printf("No solution found (unexpected)\n");
         assert(false); // Should have found a solution
     }
     
     // Cleanup
     csp_constraint_destroy(constraint);
     csp_problem_destroy(problem);
     
     // Test with no solution possible
     printf("\n=== Testing no solution case ===\n");
     
     // Create problem with impossible constraints
     problem = csp_problem_create(2, 2);
     assert(problem != NULL);
     
     // Set domains (both variables can be 0 only)
     csp_problem_set_domain(problem, 0, 1); // Only value 0
     csp_problem_set_domain(problem, 1, 1); // Only value 0
     
     // Create constraint: X0 != X1 (impossible since both must be 0)
     constraint = csp_constraint_create(2, test_constraint_diff);
     assert(constraint != NULL);
     
     // Set constraint variables
     csp_constraint_set_variable(constraint, 0, 0); // X0
     csp_constraint_set_variable(constraint, 1, 1); // X1
     
     // Add constraint to problem
     csp_problem_set_constraint(problem, 0, constraint);
     
     // Solve with forward checking
     solved = csp_problem_solve_fc(problem, solution, NULL, test_consistent);
     
     printf("Solution found: %s\n", solved ? "true" : "false");
     assert(!solved); // Should not find a solution
     
     // Cleanup
     csp_constraint_destroy(constraint);
     csp_problem_destroy(problem);
     
     printf("\nAll tests passed!\n");
     
     // Finish CSP library
     assert(csp_finish());
     
     return 0;
 }