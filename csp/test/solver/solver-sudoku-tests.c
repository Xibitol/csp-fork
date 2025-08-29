/**
 * @file solver-sudoku-tests.c
 * Tests for csp-solver.c and csp-solver-*.c
 * Uses sudoku problem
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "csp.h"
#include "util/unused.h"

static size_t backtrack_counter = 0;

#define N 9

/**
 * Checks if a number can be placed in the given cell of the Sudoku grid.
 * @param grid The Sudoku grid.
 * @param row The row index.
 * @param col The column index.
 * @param num The number to check.
 * @return true if the number can be placed, false otherwise.
 */
static bool is_safe(size_t grid[N][N], size_t row, size_t col, size_t num) {
	for (size_t x = 0; x < N; x++) {
		if (grid[row][x] == num || grid[x][col] == num) {
			return false;
		}
	}

	size_t startRow = row - row % 3, startCol = col - col % 3;
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			if (grid[i + startRow][j + startCol] == num) {
				return false;
			}
		}
	}
	return true;
}

/**
 * Solves the Sudoku grid using backtracking.
 * @param grid The Sudoku grid to solve.
 * @param row The current row to fill.
 * @param col The current column to fill.
 * @return 1 if the Sudoku is solved, 0 otherwise.
 */
static int generator_solve_sudoku(size_t grid[N][N], size_t row, size_t col) {
	if (row == N - 1 && col == N) {
		return 1;
	}
	if (col == N) {
		row++;
		col = 0;
	}
	if (grid[row][col] != 0) {
		return generator_solve_sudoku(grid, row, col + 1);
	}
	for (size_t num = 1; num <= N; num++) {
		if (is_safe(grid, row, col, num)) {
			grid[row][col] = num;
			if (generator_solve_sudoku(grid, row, col + 1)) {
				return 1;
			}
		}
		grid[row][col] = 0;
	}
	return 0;
}

/**
 * Fills the diagonal boxes of the Sudoku grid with random numbers.
 * @param grid The Sudoku grid to fill.
 */
static void fill_diagonal(size_t grid[N][N]) {
	for (size_t i = 0; i < N; i += 3) {
		for (size_t j = 0; j < 3; j++) {
			for (size_t k = 0; k < 3; k++) {
				size_t num;
				do {
					num = rand() % 9 + 1;
				} while (!is_safe(grid, i + j, i + k, num));
				grid[i + j][i + k] = num;
			}
		}
	}
}

/**
 * Generates a solved Sudoku grid using a backtracking algorithm.
 * @param grid The Sudoku grid to fill.
 */
static void generate_sudoku(size_t grid[N][N]) {
	srand(time(0) ^ getpid() ^ (clock() << 16));
	fill_diagonal(grid);
	generator_solve_sudoku(grid, 0, 0);
}

/**
 * Removes random numbers from the Sudoku grid to create a puzzle.
 * @param grid The Sudoku grid to modify.
 * @param amount The number of cells to remove.
 */
static void remove_numbers(size_t grid[N][N], size_t amount) {
	size_t count = 0;
	while (count < amount) {
		size_t row = rand() % N;
		size_t col = rand() % N;
		if (grid[row][col] != 0) {
			grid[row][col] = 0;
			count++;
		}
	}
}

/**
 * Loads a new Sudoku puzzle with a specified number of unknown cells.
 * @param unknown_count amount of unknown cells in the Sudoku grid.
 * @param sudoku_count number of Sudoku grids to generate.
 * @return A pointer to an array of Sudoku grids, or NULL if an error occurred.
 */
size_t **load_new_sudoku(const size_t unknown_count,
												 const size_t sudoku_count) {
	size_t **sudoku_list = malloc(sudoku_count * sizeof(size_t *));
	if (sudoku_list == NULL) {
		perror("malloc");
		return NULL;
	}
	for (size_t i = 0; i < sudoku_count; i++) {
		size_t *sudoku = malloc(81 * sizeof(size_t));
		if (sudoku == NULL) {
			perror("malloc");
			free(sudoku_list);
			return NULL;
		}
		size_t grid[N][N] = {0};
		// generate a solved Sudoku grid by placing random numbers in the diagonal
		// boxes and performing backtracking
		generate_sudoku(grid);
		// remove random numbers from the grid to create a puzzle
		remove_numbers(grid, unknown_count);
		for (size_t k = 0; k < N; k++) {
			for (size_t j = 0; j < N; j++) {
				sudoku[k * N + j] = grid[k][j];
			}
		}
		sudoku_list[i] = sudoku;
	}
	return sudoku_list;
}

/**
 * Merges the values of the unknowns with the starter grid to create a solved
 * grid. Used solely for printing the solution at the end
 * @param output size_t* to store the merged sudoku values
 * @param values array of filled unknowns to be merged
 * @param data starter grid of the sudoku, 0s are unknowns
 */
static void merge_sudoku_values(size_t *output, const size_t *values,
																const size_t *data) {
	int value_index = 0;
	for (int data_index = 0; data_index < 81; data_index++) {
		if (data[data_index] == 0) {
			output[data_index] = values[value_index] + 1;
			value_index++;
		} else {
			output[data_index] = data[data_index];
		}
	}
}

static void print_sudoku_solution(const size_t *sudoku_grid) {
	printf("┌─────────┬─────────┬─────────┐\n");
	for (size_t row = 0; row < 9; row++) {
		printf("│");
		for (size_t col = 0; col < 9; col++) {
			if (sudoku_grid[row * 9 + col] == 0) {
				printf("  ");
			} else {
				printf(" %zu", sudoku_grid[row * 9 + col]);
			}
			if (col % 3 == 2) {
				printf(" │");
			} else {
				printf(" ");
			}
		}
		printf("\n");
		if (row % 3 == 2 && row != 8) {
			printf("├─────────┼─────────┼─────────┤\n");
		}
	}
	printf("└─────────┴─────────┴─────────┘\n");
}

/**
 * Store the characteristics of the unknowns for the construction of the
 * CSPProblem
 */
typedef struct {
	size_t index;	 // index of the unknown in the unknown list
	size_t x;			 // x coordinate of the unknown in the starter grid
	size_t y;			 // y coordinate of the unknown in the starter grid
} Unknown;

typedef struct {
	size_t count;
	size_t index;
} Location;

static Location *locations;

/**
 * Fills the unknown_positions array with the positions of the unknowns in the
 * grid.
 * @param grid starter grid of the sudoku, 0s are unknowns
 * @param unknown_positions array of Unknowns to fill with the positions of the
 * unknowns
 */
static void get_unknown_positions(const size_t *grid,
																	Unknown *unknown_positions) {
	size_t index = 0;
	for (size_t i = 0; i < 81; i++) {
		if (grid[i] == 0) {
			Unknown unknown = {index, i % 9, i / 9};
			unknown_positions[index] = unknown;
			index++;
		}
	}
}

/**
 * Get the constraining unknowns for a given unknown. The constraining unknowns
 * are the unknowns that are in the same row, column or box as the given
 * unknown.
 * @param unknown_positions array of Unknowns to fill with the positions of the
 * unknowns
 * @param index index of the unknown in the unknown list
 * @param total_unknowns total number of unknowns in the grid
 * @param constraining_unknowns array of Unknowns to fill with the positions of
 * the constraining unknowns
 * @return number of constraining unknowns found
 */
static size_t get_constraining_unknowns(const Unknown *unknown_positions,
																				const size_t index,
																				const size_t total_unknowns,
																				Unknown *constraining_unknowns) {
	size_t constraining_unknown_count = 0;
	for (size_t i = index + 1; i < total_unknowns; i++) {
		size_t x = unknown_positions[i].x;
		size_t y = unknown_positions[i].y;
		size_t x_index = unknown_positions[index].x;
		size_t y_index = unknown_positions[index].y;
		if ((x / 3 == x_index / 3 && y / 3 == y_index / 3) /*box*/ ||
				x == x_index /*row*/ || y == y_index /*column*/) {
			// if in same box, row or column, add to list of unknowns
			// unknown_positions[index] is affected by
			constraining_unknowns[constraining_unknown_count++] =
					unknown_positions[i];
		}
	}
	return constraining_unknown_count;
}

void sudoku_unknown_checklist(const CSPProblem *csp, CSPConstraint **checklist,
															size_t *amount, const size_t index,
															FilledVariables *fv) {
	size_t next = filled_variables_next_filled(fv, 0);
	while (next != SIZE_MAX) {
		if (next < index) {
			for (size_t i = 0; i < locations[next].count; i++) {
				size_t current_index = locations[next].index + i;
				CSPConstraint *constraint =
						csp_problem_get_constraint(csp, current_index);
				if (csp_constraint_get_variable(constraint, 1) == index) {
					checklist[(*amount)++] = constraint;
				}
			}
		} else if (next > index) {
			for (size_t i = 0; i < locations[index].count; i++) {
				size_t current_index = locations[index].index + i;
				CSPConstraint *constraint =
						csp_problem_get_constraint(csp, current_index);
				if (csp_constraint_get_variable(constraint, 1) == next) {
					checklist[(*amount)++] = constraint;
				}
			}
		}
		next = filled_variables_next_filled(fv, next + 1);
	}
}

void sudoku_data_checklist(const CSPProblem *csp, CSPConstraint **checklist,
													 size_t *amount, const size_t index) {
	*amount = 1;
	checklist[0] = csp_problem_get_constraint(csp, index);
}

bool unknown_checker(const CSPConstraint *constraint, const size_t *values,
										 const void *UNUSED_VAR(data)) {
	size_t unknown1 = values[csp_constraint_get_variable(constraint, 0)];
	size_t unknown2 = values[csp_constraint_get_variable(constraint, 1)];
	return unknown1 != unknown2;
}

bool data_checker(const CSPConstraint *constraint, const size_t *values,
									const void *data) {
	// csp_constraint_get_variable(constraint, csp_constraint_get_arity()-1) is
	// the coordinate of the unknown cell itself in the unknown list
	// csp_constraint_get_variable(constraint, i) is the coordinate of a cell in
	// the grid that is in the row, column or box of the unknown cell if it is
	// <81 if it is >=81, it corresponds to it's index with values
	size_t *grid = (size_t *)data;
	const size_t arity = csp_constraint_get_arity(constraint);
	const size_t current_unknown =
			csp_constraint_get_variable(constraint, arity - 1);
	for (size_t i = 0; i < arity - 1;
			 i++) {	 // arity-1 because the last variable is the unknown cell itself
		const size_t variable = csp_constraint_get_variable(constraint, i);
		if (variable < 81) {
			if (grid[variable] - 1 == values[current_unknown]) {
				return false;
			}
		} else if (values[variable - 81] == values[current_unknown]) {
			return false;
		}
	}
	return true;
	;
}

static CSPConstraint *create_data_constraint(size_t constraint_index,
																						 const Unknown *unknown_positions,
																						 const size_t *starter_grid) {
	size_t variables[21];
	const size_t x = unknown_positions[constraint_index].x;
	const size_t y = unknown_positions[constraint_index].y;
	size_t constraint_arity_index = 0;

	for (size_t i = 0; i < 9; i++) {
		if (starter_grid[i * 9 + x] != 0) {
			variables[constraint_arity_index] = i * 9 + x;	// column
			constraint_arity_index++;
		}
		if (starter_grid[y * 9 + i] != 0) {
			variables[constraint_arity_index] = y * 9 + i;	// row
			constraint_arity_index++;
		}
	}

	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			if (!(x % 3 == i || y % 3 == j) /*not already added earlier*/
					&& starter_grid[x - x % 3 + (y - y % 3) * 9 + i + j * 9] != 0) {
				variables[constraint_arity_index] =
						x - x % 3 + (y - y % 3) * 9 + i + j * 9;	// box
				constraint_arity_index++;
			}
		}
	}

	CSPConstraint *constraint =
			csp_constraint_create(constraint_arity_index + 1, data_checker);
	for (size_t i = 0; i < constraint_arity_index; i++) {
		csp_constraint_set_variable(constraint, i, variables[i]);
	}
	// the last variable is the unknown cell itself, so the checker function can
	// use it for its logic
	csp_constraint_set_variable(constraint, constraint_arity_index,
															constraint_index);
	return constraint;
}

bool solve_sudoku(const size_t *starter_grid, SolveType solve_type) {
	print_sudoku_solution(starter_grid);

	size_t unknown_count = 0;
	for (size_t i = 0; i < 81; i++) {
		if (starter_grid[i] == 0) {
			unknown_count++;
		}
	}

	// array to contain all the unknowns we'll be testing through in the
	// solver
	size_t *unknowns = malloc(unknown_count * sizeof(size_t));
	if (unknowns == NULL) {
		perror("malloc");
		return EXIT_FAILURE;
	}

	Unknown *unknown_positions = malloc(unknown_count * sizeof(Unknown));
	if (unknown_positions == NULL) {
		perror("malloc");
		free(unknowns);
		return EXIT_FAILURE;
	}
	get_unknown_positions(starter_grid, unknown_positions);

	Unknown *constraining_unknowns = malloc(21 * sizeof(Unknown));
	if (constraining_unknowns == NULL) {
		perror("malloc");
		free(unknowns);
		free(unknown_positions);
		return EXIT_FAILURE;
	}

	CSPConstraint *data_constraints[unknown_count];
	// for each unknown, create a data constraint
	for (size_t constraint_index = 0; constraint_index < unknown_count;
			 constraint_index++) {
		data_constraints[constraint_index] = create_data_constraint(
				constraint_index, unknown_positions, starter_grid);
	}

	CSPConstraint *unknown_constraints[unknown_count * 20];
	size_t total_unknown_constraints = 0;

	locations = malloc(unknown_count * sizeof(Location));

	// for each unknown, create constraints with all other affected unknowns
	for (size_t unknown_index = 0; unknown_index < unknown_count;
			 unknown_index++) {
		size_t constraining_unknown_count = get_constraining_unknowns(
				unknown_positions, unknown_index, unknown_count, constraining_unknowns);
		locations[unknown_index].count = constraining_unknown_count;
		locations[unknown_index].index = unknown_count + total_unknown_constraints;
		total_unknown_constraints += constraining_unknown_count;

		for (size_t i = 0; i < constraining_unknown_count; i++) {
			CSPConstraint *constraint = csp_constraint_create(2, unknown_checker);
			csp_constraint_set_variable(constraint, 0, unknown_index);
			csp_constraint_set_variable(constraint, 1,
																	constraining_unknowns[i].index);
			unknown_constraints[total_unknown_constraints -
													constraining_unknown_count + i] = constraint;
		}
		constraining_unknown_count = 0;
	}
	free(constraining_unknowns);

	CSPProblem *problem = csp_problem_create(
			unknown_count, unknown_count + total_unknown_constraints);
	for (size_t i = 0; i < unknown_count; i++) {
		csp_problem_set_domain(problem, i, 9);
		csp_problem_set_constraint(problem, i, data_constraints[i]);
	}
	for (size_t i = 0; i < total_unknown_constraints; i++) {
		csp_problem_set_constraint(problem, unknown_count + i,
															 unknown_constraints[i]);
	}

	// Solve the CSP problem
	bool result = csp_problem_solve(problem, unknowns, starter_grid, solve_type,
																	sudoku_unknown_checklist,
																	sudoku_data_checklist, &backtrack_counter);

	// Destroy the CSP problem
	for (size_t index = 0; index < csp_problem_get_num_constraints(problem);
			 index++) {
		csp_constraint_destroy(csp_problem_get_constraint(problem, index));
	}
	csp_problem_destroy(problem);

	if (result) {
		size_t *solution = calloc(81, sizeof(size_t));
		if (solution != NULL) {
			merge_sudoku_values(solution, unknowns, starter_grid);
			print_sudoku_solution(solution);
			free(solution);
		} else
			perror("calloc");
	} else {
		printf("No solution found\n");
	}

	free(unknown_positions);
	free(unknowns);
	free(locations);

	return result;
}

int test_solver_solver_sudoku_tests(void) {
	csp_init();
	{
		SolveType basic = 0;
		SolveType fc = FC;
		SolveType basicMRV = MRV;
		SolveType fcMRV = FC | MRV;
		for (int i = 0; i < 5; i++) {
			size_t **new_sudoku = load_new_sudoku(i * 10, 1);
			solve_sudoku(new_sudoku[0], basic);
			solve_sudoku(new_sudoku[0], fc);
			solve_sudoku(new_sudoku[0], basicMRV);
			solve_sudoku(new_sudoku[0], fcMRV);
			free(new_sudoku[0]);
			free(new_sudoku);
		}
	}
	csp_finish();
	return EXIT_SUCCESS;
}
