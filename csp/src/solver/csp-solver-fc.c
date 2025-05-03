/**
* @file csp-solver-fc.c
 * Library CSP solving functions with forward checking
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "solver/csp-solver-fc.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/csp-constraint.h"
#include "core/csp-problem.h"

// PRIVATE
typedef struct {
	size_t amount;
	size_t values[];
}Domain;

void print_domains(Domain **domains, size_t num_domains) {
	for (size_t i = 0; i < num_domains; i++) {
		printf("Domain %zu: ", i);
		for (size_t j = 0; j < domains[i]->amount; j++) {
			printf("%zu ", domains[i]->values[j]);
		}
		printf("\n");
	}
	printf("\n");
}

void print_values(size_t* values, size_t num_domains) {
	for (size_t i = 0; i < num_domains; i++) {
		printf("%zu ", values[i]);
	}
	printf("\n");
}

static int backtrack_counter = 0;

typedef struct {
  size_t domain_index;
  size_t value;
} DomainChange;

bool csp_problem_forward_check(const CSPProblem *csp, size_t *values,
  const void *data, size_t index, CSPChecklist *checklist, Domain **domains,
  DomainChange *change_stack, size_t *stack_top, size_t stack_start
) {
  CSPConstraint** variable_checks = malloc(sizeof(CSPConstraint*) * csp_problem_get_num_constraints(csp));
  if (variable_checks == NULL) {
    perror("malloc");
    return false;
  }

  for (size_t i = index + 1; i < csp_problem_get_num_domains(csp); i++) {
    size_t v_amount = 0;
    checklist(csp, variable_checks, &v_amount, i);

    CSPConstraint *relevant_check = NULL;
    for (size_t check_i = 0; check_i < v_amount; check_i++) {
      if (csp_constraint_get_variable(variable_checks[check_i], 0) == index &&
          csp_constraint_get_variable(variable_checks[check_i], 1) == i) {
        relevant_check = variable_checks[check_i];
        break;
      }
    }

    if (relevant_check == NULL) {
      continue;
    }

    for (size_t j = 0; j < domains[i]->amount; j++) {
      values[i] = domains[i]->values[j];

      if (!csp_constraint_get_check(relevant_check)(relevant_check, values, data)) {
        // Record the change in the stack
        change_stack[*stack_top].domain_index = i;
        change_stack[*stack_top].value = domains[i]->values[j];
        (*stack_top)++;

        // Remove the value from the domain
        domains[i]->amount--;
        for (size_t k = j; k < domains[i]->amount; k++) {
          domains[i]->values[k] = domains[i]->values[k + 1];
        }
        j--;
      }
    }

    if (domains[i]->amount == 0) {
      // Restore domains from the stack
      while (*stack_top > stack_start) {
        (*stack_top)--;
        size_t domain_index = change_stack[*stack_top].domain_index;
        size_t value = change_stack[*stack_top].value;

        domains[domain_index]->values[domains[domain_index]->amount] = value;
        domains[domain_index]->amount++;
      }
      free(variable_checks);
      return false;
    }
  }

  free(variable_checks);
  return true;
}

bool csp_problem_backtrack_fc(const CSPProblem *csp,
 size_t *values, const void *data, size_t index, CSPChecklist *checklist,
 Domain **domains, DomainChange *change_stack, size_t *stack_top
) {
	assert(csp_initialised());
	backtrack_counter++;

	// If all variables are assigned, the CSP is solved
	if (index == csp_problem_get_num_domains(csp)) {
		return true;
	}

	size_t stack_start = *stack_top; // Track the stack size at the start of the call

	// Try all values in the domain of the current variable
	for (size_t i = 0; i < domains[index]->amount; i++) {
		// Assign the value to the variable
		values[index] = domains[index]->values[i];

		// print_values(values, index + 1); //DEBUG
		// print_domains(domains, csp_problem_get_num_domains(csp)); //DEBUG

		// Check if the assignment is consistent with the constraints
		if (csp_problem_is_consistent(csp, values, data, index, checklist)
		 && csp_problem_forward_check(csp, values, data, index, checklist, domains, change_stack, stack_top, *stack_top)
		 && csp_problem_backtrack_fc(csp, values, data, index + 1, checklist, domains, change_stack, stack_top)
		) {
			return true;
		}

		// Restore domains from the stack after backtracking
		while (*stack_top > stack_start) { // Only restore changes made during this call
			(*stack_top)--;
			size_t domain_index = change_stack[*stack_top].domain_index;
			size_t value = change_stack[*stack_top].value;

			domains[domain_index]->values[domains[domain_index]->amount] = value;
			domains[domain_index]->amount++;
		}
		// printf("backtracked\n"); //DEBUG
		// print_domains(domains, csp_problem_get_num_domains(csp)); //DEBUG
	}
	return false;
}

// PUBLIC
// Functions
bool csp_problem_solve_fc(const CSPProblem *csp, size_t *values, const void *data, CSPChecklist *checklist, size_t* benchmark) {
	assert(csp_initialised());

	size_t num_domains = csp_problem_get_num_domains(csp);
	Domain *domains[num_domains];
	size_t stack_capacity = 0; // Adjust as needed

	// Allocate memory for each domain
	for (size_t i = 0; i < num_domains; i++) {
		size_t domain_size = csp_problem_get_domain(csp, i);
		stack_capacity += domain_size;
		domains[i] = malloc(sizeof(Domain) + domain_size * sizeof(size_t));
		if (domains[i] == NULL) {
			perror("malloc");
			// Free previously allocated domains
			for (size_t j = 0; j < i; j++) {
				free(domains[j]);
			}
			return false;
		}
		domains[i]->amount = domain_size;
		for (size_t j = 0; j < domain_size; j++) {
			domains[i]->values[j] = j;
		}
	}

	// Initialize the change stack for forward checking
	DomainChange *change_stack = malloc(stack_capacity * sizeof(DomainChange));
	if (change_stack == NULL) {
		perror("malloc");
		for (size_t i = 0; i < num_domains; i++) {
			free(domains[i]);
		}
		return false;
	}
	size_t stack_top = 0;

	// Start the backtracking algorithm
	bool result = csp_problem_backtrack_fc(csp, values, data, 0, checklist, domains, change_stack, &stack_top);

	// Free allocated memory
	for (size_t i = 0; i < num_domains; i++) {
		free(domains[i]);
	}
	free(change_stack);

	if (benchmark != NULL) {
		benchmark[0] = backtrack_counter;
	}

	// Reset the backtrack counter
	backtrack_counter = 0;

	return result;
}
