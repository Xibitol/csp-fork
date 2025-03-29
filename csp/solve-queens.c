#include <stdio.h>
#include <stdlib.h>

#include "./csp.h"

// Check if the queens are compatible
bool queen_compatibles(CSPConstraint *constraint, const size_t *values, unsigned int *data) {
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
void print_solution(unsigned int number, size_t *queens) {
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
      for (size_t i = 0; i < number - 1; i++) {
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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <number>\n", argv[0]);
    return EXIT_FAILURE;
  }
  unsigned int number;
  if (sscanf(argv[1], "%u", &number) != 1) {
    fprintf(stderr, "Invalid number: %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  // Initialise the library
  csp_init();
  {
    // Create the queens array
    size_t *queens = calloc(number, sizeof(size_t));

    // Create the CSP problem
    size_t index;
    CSPProblem *problem = csp_problem_create(number, number * (number - 1) / 2);
    for (size_t i = 0; i < number; i++) {
      csp_problem_set_domain(problem, i, number);
    }
    index = 0;
    for (size_t i = 0; i < number - 1; i++) {
      for (size_t j = i + 1; j < number; j++) {
        csp_problem_set_constraint(problem, index, csp_constraint_create(2, (CSPChecker *)queen_compatibles));
        csp_constraint_set_variable(csp_problem_get_constraint(problem, index), 0, i);
        csp_constraint_set_variable(csp_problem_get_constraint(problem, index), 1, j);
        index++;
      }
    }

    // Solve the CSP problem
    bool result = csp_problem_solve(problem, queens, NULL);

    // Destroy the CSP problem
    while (index--) {
      csp_constraint_destroy(csp_problem_get_constraint(problem, index));
    }
    csp_problem_destroy(problem);

    // Print the solution
    if (result) {
      print_solution(number, queens);
    } else {
      printf("No solution found\n");
    }

    // Free the queens array
    free(queens);
  }
  // Finish the library
  csp_finish();

  return EXIT_SUCCESS;
}
