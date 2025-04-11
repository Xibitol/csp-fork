//
// Created by adrien on 26/03/25.
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "csp.h"

static size_t backtrack_counter = 0;

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

void get_unknown_positions(const size_t *grid, size_t *unknown_positions) {
  int index = 0;
  for (int i = 0; i < 81; i++) {
    if (grid[i] == 9) {
      unknown_positions[index] = i;
      index++;
    }
  }
}

void print_sudoku_solution(const size_t *sudoku_grid) {
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

  for (int i = 0; i < 20; i++) {
    // csp_constraint_get_variable(constraint, 20) is the coordinate of the unknown cell itself
    // csp_constraint_get_variable(constraint, i) is the coordinate of a cell in the grid that is in the row, column or box of the unknown cell
    if (grid[csp_constraint_get_variable(constraint, 20)] == grid[csp_constraint_get_variable(constraint, i)]) {
      free(grid);
      return false;
    }
  }
  free(grid);
  return true;
}

void load_sudoku(const char* str_sudoku, size_t* grid, size_t *unknown_count) {
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

bool sudoku_consistent(const CSPProblem *csp, size_t *values, const void *data, size_t index) {
  // Only increase the backtrack counter if the value is 0, because we want to count the number of nodes explored,
  // we can know we're on a new node when we set a value to 0, otherwise we are just testing other values for the node we're already on
  if (values[index-1] == 0) {
    backtrack_counter++;
  }
  CSPConstraint *constraint = csp_problem_get_constraint(csp, index-1);
  if (csp_constraint_get_check(constraint)(constraint, values, data)) {
    return true;
  }
  values[index-1] = 9; // reset the value to 9
  return false;
}

int solve_sudoku(size_t* starter_grid) {

  // amount of unknowns in the grid
  print_sudoku_solution(starter_grid);
  size_t unknown_count = 0;
  for (size_t i = 0; i < 81; i++) {
      if (starter_grid[i] == 9) {
          unknown_count++;
      }
  }

  // Initialise the library
  csp_init();
  {
    // array to contain all the unknowns we'll be testing through in the solver
    size_t *unknowns = calloc(unknown_count, sizeof(size_t));
    if (unknowns == NULL) {
      perror("calloc failed");
      return EXIT_FAILURE;
    }
    for (size_t i = 0; i < unknown_count; i++) {
      unknowns[i] = 9; // set to 9, keeps 0 available for the values within domain
    }

    size_t *unknown_positions = calloc(unknown_count, sizeof(size_t));
    if (unknown_positions == NULL) {
      perror("calloc failed");
      return EXIT_FAILURE;
    }
    get_unknown_positions(starter_grid, unknown_positions);

    CSPProblem *problem = csp_problem_create(unknown_count, unknown_count);
    for (size_t i = 0; i < unknown_count; i++) {
      csp_problem_set_domain(problem, i, 9); // Each unknown cell can have values 0-8
    }

    // one constraint per unknown cell, each constraint checks unknown value is unique in row, column and box
    for (size_t constraint_index = 0; constraint_index < unknown_count; constraint_index++) {
      // arity = 9 from box + 6 from row + 6 from column
      CSPConstraint *constraint = csp_constraint_create(21, checker);

      // coords of the unknown cell corresponding to the constraint
      size_t x = unknown_positions[constraint_index] % 9;
      size_t y = unknown_positions[constraint_index] / 9;
      size_t row_cell_counter = 0;
      size_t column_cell_counter = 0;
      for (size_t i = 0; i < 9; i++) {
        if (i != y) {
          csp_constraint_set_variable(constraint, column_cell_counter + 8, i*9 + x); // column
          column_cell_counter++;
        }
        if (i != x) {
          csp_constraint_set_variable(constraint, row_cell_counter, y*9 + i); // row
          row_cell_counter++;
        }
      }
      size_t box_cell_counter = 0;
      for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
          // set the variable in the constraint
          if (x%3 == i || y%3 == j) { //variable already set in either row or column
            continue;
          }
          assert(box_cell_counter < 4);
          csp_constraint_set_variable(constraint, 16 + box_cell_counter, x-x%3 + (y-y%3)*9 + i + j*9); // box
          box_cell_counter++;
        }
      }
      csp_constraint_set_variable(constraint, 20, unknown_positions[constraint_index]); // unknown cell itself
      csp_problem_set_constraint(problem, constraint_index, constraint);
    }

    FILE* file = fopen("sudoku_benchmark.txt", "a");

    // Start the timer
    clock_t start_time = clock();

    // Solve the CSP problem
    bool result = csp_problem_solve(problem, unknowns, starter_grid, sudoku_consistent);

    // Stop the timer
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    fprintf(file, "%f %zu\n", time_spent, backtrack_counter);

    fclose(file);
    backtrack_counter = 0;

    // Destroy the CSP problem
    for (size_t index = 0; index < unknown_count; index++) {
      csp_constraint_destroy(csp_problem_get_constraint(problem, index));
    }
    csp_problem_destroy(problem);

    // Print the solution
    if (result) {
      size_t* solution = calloc(81, sizeof(size_t));
      if (solution == NULL) {
        perror("calloc failed");
        free(unknowns);
        free(starter_grid);
        free(unknown_positions);
        return EXIT_FAILURE;
      }
      merge_sudoku_values(solution, unknowns, starter_grid);
      print_sudoku_solution(solution);
      free(solution);
    } else {
      printf("No solution found\n");
    }

    // Free allocated arrays
    free(unknowns);
    free(starter_grid);
    free(unknown_positions);
  }
  // Finish the library
  csp_finish();

  return EXIT_SUCCESS;
}
