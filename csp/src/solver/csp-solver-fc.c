/**
 * @file csp-solver-fc.c
 * Defines functions to solve a CSP problem using backtracking algorithm with forward checking.
 *
 * @author vgegaden
 * @date 30/04/2025
 */

 #include "solver/csp-solver-fc.h"

 #include <stdbool.h>
 #include <stddef.h>
 #include <stdlib.h>
 #include <assert.h>
 #include <stdio.h>
 #include <string.h>
 
 #include "core/csp-lib.h"
 #include "core/csp-problem.h"
 #include "core/csp-constraint.h"
 
 
 // Forward checking function
 static bool forward_check(const CSPProblem *csp, const size_t *values, 
                          size_t index, bool **domains) {
     // For each constraint
     for (size_t i = 0; i < csp_problem_get_num_constraints(csp); i++) {
         CSPConstraint *constraint = csp_problem_get_constraint(csp, i);
         size_t arity = csp_constraint_get_arity(constraint);
         
         // Check if the constraint involves the current variable
         bool involves_current = false;
         for (size_t j = 0; j < arity; j++) {
             if (csp_constraint_get_variable(constraint, j) == index) {
                 involves_current = true;
                 break;
             }
         }
         
         if (!involves_current) continue;
         
         // For each variable in the constraint that is not yet assigned
         for (size_t j = 0; j < arity; j++) {
             size_t var = csp_constraint_get_variable(constraint, j);
             if (var <= index) continue; // Skip assigned variables
             
             // For each possible value in the domain of the future variable
             for (size_t val = 0; val < csp_problem_get_domain(csp, var); val++) {
                 if (!domains[var][val]) continue; // Skip already eliminated values
                 
                 // Create a temporary assignment to test consistency
                 size_t *temp_values = malloc(csp_problem_get_num_domains(csp) * sizeof(size_t));
                 if (!temp_values) return false;
                 
                 // Copy current assignments
                 for (size_t k = 0; k < csp_problem_get_num_domains(csp); k++) {
                     if (k <= index) {
                         temp_values[k] = values[k];
                     } else if (k == var) {
                         temp_values[k] = val;
                     } else {
                         temp_values[k] = 0; // Arbitrary, won't be checked
                     }
                 }
                 
                 // Check if this assignment would violate the constraint
                 if (!csp_constraint_get_check(constraint)(constraint, temp_values, NULL)) {
                     domains[var][val] = false; // Eliminate this value
                 }
                 
                 free(temp_values);
             }
             
             // Check if the domain is now empty
             bool domain_empty = true;
             for (size_t val = 0; val < csp_problem_get_domain(csp, var); val++) {
                 if (domains[var][val]) {
                     domain_empty = false;
                     break;
                 }
             }
             
             if (domain_empty) {
                 return false; // Forward checking detected a dead end
             }
         }
     }
     
     return true;
 }
 

 // Backtracking with forward checking
 bool csp_problem_backtrack_fc(const CSPProblem *csp, size_t *values, 
                             const void *data, size_t index, 
                             CSPConsistentFC is_consistent, bool **domains) {
     assert(csp_initialised());
     
     // If all variables are assigned, the CSP is solved
     if (index == csp_problem_get_num_domains(csp)) {
         return true;
     }
     
     // Try all values in the domain of the current variable
     for (size_t i = 0; i < csp_problem_get_domain(csp, index); i++) {
         // Skip values that have been eliminated by forward checking
         if (!domains[index][i]) continue;
         
         // Assign the value to the variable
         values[index] = i;
         
         // Make a copy of the current domains for restoration if needed
         bool **new_domains = malloc(csp_problem_get_num_domains(csp) * sizeof(bool *));
         if (!new_domains) return false;
         
         for (size_t j = 0; j < csp_problem_get_num_domains(csp); j++) {
             new_domains[j] = malloc(csp_problem_get_domain(csp, j) * sizeof(bool));
             if (!new_domains[j]) {
                 for (size_t k = 0; k < j; k++) free(new_domains[k]);
                 free(new_domains);
                 return false;
             }
             memcpy(new_domains[j], domains[j], csp_problem_get_domain(csp, j) * sizeof(bool));
         }
         
         // Perform forward checking
         if (forward_check(csp, values, index, new_domains)) {
             // Continue with the next variable
             if (csp_problem_backtrack_fc(csp, values, data, index + 1, is_consistent, new_domains)) {
                 // Free the domains before returning
                 for (size_t j = 0; j < csp_problem_get_num_domains(csp); j++) {
                     free(new_domains[j]);
                 }
                 free(new_domains);
                 return true;
             }
         }
         
         // Restore domains for next iteration
         for (size_t j = 0; j < csp_problem_get_num_domains(csp); j++) {
             free(new_domains[j]);
         }
         free(new_domains);
     }
     
     return false;
 }
 
 // Main solving function with forward checking
 bool csp_problem_solve_fc(const CSPProblem *csp, size_t *values, 
                          const void *data, CSPConsistentFC is_consistent) {
     assert(csp_initialised());
     
     // Initialize domains - all values are initially possible
     bool **domains = malloc(csp_problem_get_num_domains(csp) * sizeof(bool *));
     if (!domains) return false;
     
     for (size_t i = 0; i < csp_problem_get_num_domains(csp); i++) {
         domains[i] = malloc(csp_problem_get_domain(csp, i) * sizeof(bool));
         if (!domains[i]) {
             for (size_t j = 0; j < i; j++) free(domains[j]);
             free(domains);
             return false;
         }
         for (size_t j = 0; j < csp_problem_get_domain(csp, i); j++) {
             domains[i][j] = true;
         }
     }
     
     // Start the backtracking algorithm with forward checking
     bool result = csp_problem_backtrack_fc(csp, values, data, 0, is_consistent, domains);
     
     // Free the domains
     for (size_t i = 0; i < csp_problem_get_num_domains(csp); i++) {
         free(domains[i]);
     }
     free(domains);
     
     return result;
 }