/**
 * @file csp-solver-fc.h
 * Defines functions to solve a CSP problem using backtracking algorithm with forward checking.
 *
 * @author vgegaden
 * @date 30/04/2025
 */

 #pragma once

 #include <stdbool.h>
 #include <stddef.h>
 
 #include "core/csp-problem.h"
 
 
 typedef bool (*CSPConsistentFC)(const CSPProblem *, 
                                const size_t *, 
                                const void *, 
                                size_t, 
                                bool **);
 
 
 bool csp_problem_backtrack_fc(const CSPProblem *csp, size_t *values, 
                              const void *data, size_t index, 
                              CSPConsistentFC is_consistent, bool **domains);
                              
 bool csp_problem_solve_fc(const CSPProblem *csp, size_t *values, 
                          const void *data, CSPConsistentFC is_consistent);