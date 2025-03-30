//
// Created by adrien on 26/03/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "../csp.h"
#include "../solver/sudoku-basic-solve.h"

void merge_sudoku_values(size_t *output, const size_t *values, const size_t *data) {
  int value_index = 0;
  for (int data_index = 0; data_index < 81; data_index++) {
    if (data[data_index] == 9 && values[value_index] != 9) {
      output[data_index] = values[value_index];
      value_index++;
    } else {
      output[data_index] = data[data_index];
    }
  }
}

void print_solution(const size_t *sudoku_grid) {
  printf("┌─────────┬─────────┬─────────┐\n");
  for (size_t row = 0; row < 9; row++) {
    printf( "│");
    for (size_t col = 0; col < 9; col++) {
      if (sudoku_grid[row * 9 + col] == 9) {
        printf( "  ");
      } else {
        printf( " %zu", sudoku_grid[row * 9 + col] + 1);
      }
      if (col % 3 == 2) {
        printf( " │");
      } else {
        printf( " ");
      }
    }
    printf( "\n");
    if (row % 3 == 2 && row != 8) {
      printf( "├─────────┼─────────┼─────────┤\n");
    }
  }
  printf( "└─────────┴─────────┴─────────┘\n");
}

bool checker(const CSPConstraint *constraint, const size_t *values, const void *data) {
  //data is the starter grid, values are the unknowns
  //we need to merge the two into a single grid before checking the constraints
  size_t* grid = calloc(81, sizeof(size_t));
  if (grid == NULL) {
    perror("calloc failed");
    return false;
  }
  merge_sudoku_values(grid, values, data);

  for (int i = 0; i < csp_constraint_get_arity(constraint) - 1; i++) {
    for (int j = i + 1; j < csp_constraint_get_arity(constraint); j++) {
      if (grid[csp_constraint_get_variable(constraint, i)] == 9 || grid[csp_constraint_get_variable(constraint, j)] == 9) {
        continue;
      }
      if (grid[csp_constraint_get_variable(constraint, i)] == grid[csp_constraint_get_variable(constraint, j)]) {
        free(grid);
        return false;
      }
    }
  }
  free(grid);
  return true;
}

void load_sudoku(const char* str_sudoku, size_t* grid, int *unknown_count) {
  if (str_sudoku == NULL || grid == NULL || unknown_count == NULL) {
    fprintf(stderr, "Invalid arguments\n");
    return;
  }
  for (size_t i = 0; i < 81; i++) {
    if (str_sudoku[i] == '\0') {
      perror("Invalid sudoku string");
      return;
    }
    grid[i] = str_sudoku[i] - '0';
    if (grid[i] == 0) {
      *unknown_count += 1;
      grid[i] = 9;
    } else {
      grid[i] = grid[i] - 1;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <sudoku>\n", argv[0]);
    return EXIT_FAILURE;
  }

  int unknown_count = 0;
  size_t *starter_grid = calloc(81, sizeof(size_t));
  if (starter_grid == NULL) {
    perror("calloc failed");
    return EXIT_FAILURE;
  }
  load_sudoku(argv[1], starter_grid ,&unknown_count);

  // amount of unknowns in the grid
  print_solution(starter_grid);

  // Initialise the library
  csp_init();
  {
    // array to contain all the unknowns we'll be testing through in the solver
    size_t *values = calloc(unknown_count, sizeof(size_t));
    if (values == NULL) {
      perror("calloc failed");
      return EXIT_FAILURE;
    }
    for (size_t i = 0; i < unknown_count; i++) {
      values[i] = 9; // set to 9, keeps 0 available for the values within domain
    }

    CSPProblem *problem = csp_problem_create(unknown_count, 27);
    for (size_t i = 0; i < unknown_count; i++) {
      csp_problem_set_domain(problem, i, 9); // Each unknown cell can have values 0-8
    }

    // row constraints
    for (size_t i = 0; i < 9; i++) {
      CSPConstraint *row_constraint = csp_constraint_create(9, checker); // one constraint per row
      for (size_t j = 0; j < 9; j++) {
        // variables are the indexes of all cells affected by this constraint
        csp_constraint_set_variable(row_constraint, j, i * 9 + j);
      }
      csp_problem_set_constraint(problem, i, row_constraint);
    }

    // column constraints
    for (size_t i = 0; i < 9; i++) {
      CSPConstraint *column_constraint = csp_constraint_create(9, checker); // one constraint per column
      for (size_t j = 0; j < 9; j++) {
        // variables are the indexes of all cells affected by this constraint
        csp_constraint_set_variable(column_constraint, j, i + j * 9);
      }
      csp_problem_set_constraint(problem, 9 + i, column_constraint);
    }

    // box constraints
    for (size_t box_row = 0; box_row < 3; box_row++) {
      for (size_t box_col = 0; box_col < 3; box_col++) {
        CSPConstraint *box_constraint = csp_constraint_create(9, checker); // one constraint per box
        for (size_t i = 0; i < 3; i++) {
          for (size_t j = 0; j < 3; j++) {
            size_t index = (box_row * 3 + i) * 9 + (box_col * 3 + j);
            csp_constraint_set_variable(box_constraint, i * 3 + j, index);
          }
        }
        csp_problem_set_constraint(problem, 18 + box_row * 3 + box_col, box_constraint);
      }
    }

    FILE* file = fopen("sudoku_benchmark.txt", "a");

    // Start the timer
    clock_t start_time = clock();

    // Solve the CSP problem
    bool result = sudoku_basic_solve(problem, values, starter_grid);

    // Stop the timer
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    fprintf(file, "%f %zu\n", time_spent, get_backtrack_counter());

    fclose(file);
    reset_backtrack_counter();

    // Destroy the CSP problem
    for (size_t index = 0; index < 27; index++) {
      csp_constraint_destroy(csp_problem_get_constraint(problem, index));
    }
    csp_problem_destroy(problem);

    // Print the solution
    if (result) {
      size_t* solution = calloc(81, sizeof(size_t));
      if (solution == NULL) {
        perror("calloc failed");
        return EXIT_FAILURE;
      }
      merge_sudoku_values(solution, values, starter_grid);
      print_solution(solution);
      free(solution);
    } else {
      printf("No solution found\n");
    }

    // Free allocated arrays
    free(values);
    free(starter_grid);
  }
  // Finish the library
  csp_finish();

  return EXIT_SUCCESS;
}
