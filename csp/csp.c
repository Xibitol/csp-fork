/**
 * @file csp.c
 * @brief Source file for the CSP (Constraint Satisfaction Problem) project.
 *
 * This file contains the implementation of the CSP solver whose content has been partially
 * generated using copilot.
 *
 * The CSP solver is a backtracking algorithm that solves a CSP by
 * assigning values to variables from their domains and checking
 * if the assignment is consistent.
 *
 * @author Ch. Demko
 * @date 2024-2025
 * @version 1.0
 */

#include "./csp.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./csp.inc"

static int counter = 0;

static void _verify(void) {
  assert(!csp_initialised());
}

bool csp_init(void) {
  static bool first = true;
  if (first) {
    assert(atexit(_verify) == 0);
    first = false;
  }
  if (!counter++) {
    assert(printf("CSP initialised\n"));
  }
  return true;
}

bool csp_finish(void) {
  if (counter) {
    if (!--counter) {
      assert(printf("CSP finished\n"));
    }
    return true;
  } else {
    return false;
  }
}

bool csp_initialised(void) {
  return counter > 0;
}

CSPConstraint *csp_constraint_create(size_t arity, CSPChecker *check) {
  assert(csp_initialised());
  assert(arity > 0);
  assert(check != NULL);
  assert(printf("Creating constraint with arity %lu\n", arity));
  // Allocate memory for the constraint
  CSPConstraint *constraint = malloc(sizeof(CSPConstraint) + arity * sizeof(size_t));
  if (constraint != NULL) {
    constraint->arity = arity;
    constraint->check = check;
    memset(constraint->variables, 0, arity * sizeof(size_t));
  }
  return constraint;
}

void csp_constraint_destroy(CSPConstraint *constraint) {
  assert(csp_initialised());
  assert(printf("Destroying constraint with arity %lu\n", constraint->arity));
  free(constraint);
}

size_t csp_constraint_get_arity(const CSPConstraint *constraint) {
  assert(csp_initialised());
  return constraint->arity;
}

CSPChecker *csp_constraint_get_check(const CSPConstraint *constraint) {
  assert(csp_initialised());
  return constraint->check;
}

void csp_constraint_set_variable(CSPConstraint *constraint, size_t index, size_t variable) {
  assert(csp_initialised());
  assert(index < constraint->arity);
  constraint->variables[index] = variable;
}

size_t csp_constraint_get_variable(const CSPConstraint *constraint, size_t index) {
  assert(csp_initialised());
  assert(index < constraint->arity);
  return constraint->variables[index];
}

bool csp_constraint_to_check(const CSPConstraint *constraint, size_t index) {
    assert(csp_initialised());
    for (size_t i = 0; i < constraint->arity; i++) {
      if (constraint->variables[i] >= index) {
        return false;
      }
    }
    return true;
}

CSPProblem *csp_problem_create(size_t num_domains, size_t num_constraints) {
  assert(csp_initialised());
  assert(num_domains > 0);
  assert(num_constraints > 0);
  assert(printf("Creating CSP problem with %lu domains and %lu constraints\n", num_domains, num_constraints));
  // Allocate memory for the CSP problem
  CSPProblem *csp = malloc(sizeof(CSPProblem));
  if (csp != NULL) {
    // Allocate memory for the domains of the CSP problem
    csp->domains = calloc(num_domains, sizeof(size_t));
    if (csp->domains != NULL) {
      // Allocate memory for the contraints of the CSP problem
      csp->constraints = malloc(num_constraints * sizeof(CSPConstraint *));
      if (csp->constraints != NULL) {
        for (size_t i = 0; i < num_constraints; i++) {
          csp->constraints[i] = NULL;
        }
        csp->num_domains = num_domains;
        csp->num_constraints = num_constraints;
      } else {
        free(csp->domains);
        free(csp);
        csp = NULL;
      }
    } else {
      free(csp);
      csp = NULL;
    }
  } else {
    free(csp);
    csp = NULL;
  }
  return csp;
}

void csp_problem_destroy(CSPProblem *csp) {
  assert(csp_initialised());
  assert(
      printf("Destroying CSP problem with %lu domains and %lu constraints\n", csp->num_domains, csp->num_constraints));
  free(csp->constraints);
  free(csp->domains);
  free(csp);
}

size_t csp_problem_get_num_constraints(const CSPProblem *csp) {
  assert(csp_initialised());
  return csp->num_constraints;
}

void csp_problem_set_constraint(CSPProblem *csp, size_t index, CSPConstraint *constraint) {
  assert(csp_initialised());
  assert(index < csp->num_constraints);
  assert(constraint != NULL);
#ifndef NDEBUG
  for (size_t i = 0; i < constraint->arity; i++) {
    assert(constraint->variables[i] < csp->num_domains);
  }
#endif
  csp->constraints[index] = constraint;
}

CSPConstraint *csp_problem_get_constraint(const CSPProblem *csp, size_t index) {
  assert(csp_initialised());
  assert(index < csp->num_constraints);
  return csp->constraints[index];
}

size_t csp_problem_get_num_domains(const CSPProblem *csp) {
  assert(csp_initialised());
  return csp->num_domains;
}

void csp_problem_set_domain(CSPProblem *csp, size_t index, size_t domain) {
  assert(csp_initialised());
  assert(index < csp->num_domains);
  csp->domains[index] = domain;
}

size_t csp_problem_get_domain(const CSPProblem *csp, size_t index) {
  assert(csp_initialised());
  assert(index < csp->num_domains);
  return csp->domains[index];
}

bool csp_problem_is_consistent(const CSPProblem *csp, const size_t *values, const void *data, size_t index) {
  assert(csp_initialised());
  // Check all constraints
  for (size_t i = 0; i < csp->num_constraints; i++) {
    CSPConstraint *constraint = csp->constraints[i];
    // Verify if the constraint has to be checked and check it
    if (csp_constraint_to_check(constraint, index) && !constraint->check(constraint, values, data)) {
      return false;
    }
  }
  return true;
}

bool csp_problem_solve(const CSPProblem *csp, size_t *values, const void *data) {
  assert(csp_initialised());
  return csp_problem_backtrack(csp, values, data, 0);
}

bool csp_problem_backtrack(const CSPProblem *csp, size_t *values, const void *data, size_t index) {
  assert(csp_initialised());
  // If all variables are assigned, the CSP is solved
  if (index == csp->num_domains) {
    return true;
  }
  // Try all values in the domain of the current variable
  for (size_t i = 0; i < csp->domains[index]; i++) {
    // Assign the value to the variable
    values[index] = i;
    // Check if the assignment is consistent with the constraints
    if (csp_problem_is_consistent(csp, values, data, index + 1) &&
        csp_problem_backtrack(csp, values, data, index + 1)) {
      return true;
    }
  }
  return false;
}

