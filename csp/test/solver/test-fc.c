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

 // Ajoutez ces variables globales (pour le test uniquement)
static size_t backtracking_steps = 0;
static size_t fc_steps = 0;

// Modifiez votre vérification de consistance pour incrémenter les compteurs
bool test_consistent_with_counter(const CSPProblem *csp, const size_t *values, 
                                const void *data, size_t index, bool **domains) {

                                    // Backtracking simple
backtracking_steps = 0;
csp_problem_solve(problem, solution, NULL, test_consistent_with_counter);
printf("Backtracking simple : %zu étapes\n", backtracking_steps);

// Forward Checking
fc_steps = 0;
csp_problem_solve_fc(problem, solution, NULL, test_consistent_with_counter);
printf("Forward Checking : %zu étapes\n", fc_steps);

assert(fc_steps < backtracking_steps); // Le FC doit explorer moins de possibilités
    backtracking_steps++; // Compte toutes les vérifications
    return csp_problem_is_consistent(csp, values, data, index);
}
 
 bool test_solver_test_fc(void) {
    printf("Running FC solver test...\n");
    
    assert(csp_init());
    
    CSPProblem *problem = csp_problem_create(2, 1);
    assert(problem != NULL);
    
    csp_problem_set_domain(problem, 0, 2);
    csp_problem_set_domain(problem, 1, 2);
    
    CSPConstraint *constraint = csp_constraint_create(2, test_constraint_diff);
    csp_constraint_set_variable(constraint, 0, 0);
    csp_constraint_set_variable(constraint, 1, 1);
    csp_problem_set_constraint(problem, 0, constraint);
    
    size_t solution[2] = {0};
    bool solved = csp_problem_solve_fc(problem, solution, NULL, NULL);
    
    assert(solved);
    assert(solution[0] != solution[1]);
    
    csp_constraint_destroy(constraint);
    csp_problem_destroy(problem);
    assert(csp_finish());
    
    printf("FC solver test passed!\n");
    return true;
}

// Point d'entrée principal des tests
int main_(void) {
    return test_solver_test_fc() ? 0 : 1;
}