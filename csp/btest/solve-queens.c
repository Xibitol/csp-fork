/**
* @file solve-queens.c
 * N_Queens problem solver using the CSP library to perform benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "csp.h"
#include "util/unused.h"

static size_t backtrack_counter;

// Check if the queens are compatible
bool queen_compatibles(CSPConstraint *constraint, const size_t *values,
	unsigned int *UNUSED_VAR(data)
){
  // Get the variables
  size_t x0 = csp_constraint_get_variable(constraint, 0);
  size_t x1 = csp_constraint_get_variable(constraint, 1);
  // Get the values
  size_t y0 = values[x0];
  size_t y1 = values[x1];
  // Check if the queens are compatible
  return /* x0 != x1 && */ y0 != y1 && x0 + y1 != x1 + y0 && x0 + y0 != x1 + y1;
}

// Print the solution
static void print_queens_solution(unsigned int number, const size_t *queens) {
  printf("┌");
  for (size_t i = 0; i < number - 1; i++) {
    printf("───┬");
  }
  printf("───┐\n");
  for (size_t i = 0; i < number; i++) {
    printf("│");
    for (size_t j = 0; j < number; j++) {
      if (queens[i] == j) {
        printf(" ♛ │");
      } else {
        printf("   │");
      }
    }
    if (i != number - 1) {
      printf("\n├");
      for (size_t k = 0; k < number - 1; k++) {
        printf("───┼");
      }
      printf("───┤\n");
    }
  }
  printf("\n└");
  for (size_t i = 0; i < number - 1; i++) {
    printf("───┴");
  }
  printf("───┘\n");
}

void queens_checklist(const CSPProblem *csp, CSPConstraint** checklist, size_t* amount, const size_t index) {
	*amount = index;

	size_t queen_count = csp_problem_get_num_domains(csp);
	// for 6 queens, these are the values the formula below creates per index 0 ; 1 5 ; 2 6 9 ; 3 7 10 12 ; 4 8 11 13 14
	// this pattern is necessary due to the way the constraints are created
	for (size_t i = 0; i < index; i++) {
		checklist[i] = csp_problem_get_constraint(csp, i * queen_count - i * (i + 1) / 2 + (index - i - 1));
	}
}

int solve_queens(size_t queen_count, const char* resultFile, bool forward_checking, bool silent) {
  // Initialise the library
  csp_init();
  {
    // Create the queens array
    size_t *queens = calloc(queen_count, sizeof(size_t));
    if (queens == NULL) {
      perror("calloc failed");
      return EXIT_FAILURE;
    }

    // Create the CSP problem
    size_t index;

    // num_domains is the number of queens here, also the number of columns since each queen is in a different column
    // Each constraint corresponds to a pair of queens that need to be checked for compatibility.
    // The number of constraints corresponds to the number of pairs of queens that need to be checked
    // This is equal to the combination C(n, 2) = n * (n - 1) / 2
    CSPProblem *problem = csp_problem_create(queen_count, queen_count * (queen_count - 1) / 2);
    for (size_t i = 0; i < queen_count; i++) {
      csp_problem_set_domain(problem, i, queen_count); // Domain = number of possible row positions for a queen
    }
    index = 0;
    // This way of creating the constraints ensures that each pair of queens is always from different columns
    // Otherwise we would need n * n constraints to check all pairs of queens
    for (size_t i = 0; i < queen_count - 1; i++) {
      for (size_t j = i + 1; j < queen_count; j++) {
        // arity is 2 because we are checking compatibility between two queens
        csp_problem_set_constraint(problem, index, csp_constraint_create(2, (CSPChecker *)queen_compatibles));
        csp_constraint_set_variable(csp_problem_get_constraint(problem, index), 0, i);
        csp_constraint_set_variable(csp_problem_get_constraint(problem, index), 1, j);
        index++;
      }
    }

    FILE* file = fopen(resultFile, "a");

    // Start the timer
    clock_t start_time = clock();

  	bool result;
    // Solve the CSP problem
  	if (forward_checking) {
  		result = csp_problem_solve_fc(problem, queens, NULL, queens_checklist, &backtrack_counter);
  	} else {
  		result = csp_problem_solve(problem, queens, NULL, queens_checklist, &backtrack_counter);
  	}

    // Stop the timer
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    fprintf(file, "%f %zu\n", time_spent, backtrack_counter);

    fclose(file);

    // Destroy the CSP problem
    while (index--) {
      csp_constraint_destroy(csp_problem_get_constraint(problem, index));
    }
    csp_problem_destroy(problem);

    // Print the solution
    if(!silent){
      if (result) {
        print_queens_solution(queen_count, queens);
      }else{
        printf("No solution found\n");
      }
	}

    // Free the queens array
    free(queens);
  }
  // Finish the library
  csp_finish();

  return EXIT_SUCCESS;
}
