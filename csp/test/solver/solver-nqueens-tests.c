/**
 * @file solver-nqueens-tests.c
 * Tests for csp-solver.c and csp-solver-*.c
 * Uses n_queens problem
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "csp.h"
#include "util/unused.h"

// Check if the queens are compatible
bool queen_compatibles(CSPConstraint *constraint, const size_t *values,
											 unsigned int *UNUSED_VAR(data)) {
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

void queens_checklist(const CSPProblem *csp, CSPConstraint **checklist,
											size_t *amount, const size_t index, FilledVariables *fv) {
	size_t num_queens = csp_problem_get_num_domains(csp);
	*amount = 0;
	size_t next = filled_variables_next_filled(fv, 0);
	while (next != SIZE_MAX) {
		if (next < index) {
			checklist[(*amount)++] = csp_problem_get_constraint(
					csp, next * num_queens - next * (next + 1) / 2 + (index - next) - 1);
		}
		if (next > index) {
			checklist[(*amount)++] = csp_problem_get_constraint(
					csp,
					index * num_queens - index * (index + 1) / 2 + (next - index) - 1);
		}
		next = filled_variables_next_filled(fv, next + 1);
	}
}

bool solve_queens(size_t queen_count, SolveType solve_type,
									bool test_node_counter) {
	// Create the queens array
	size_t *queens = calloc(queen_count, sizeof(size_t));
	assert(queens != NULL);

	// Create the CSP problem
	size_t index;

	// num_domains is the number of queens here, also the number of columns
	// since each queen is in a different column Each constraint corresponds
	// to a pair of queens that need to be checked for compatibility. The
	// number of constraints corresponds to the number of pairs of queens
	// that need to be checked This is equal to the combination
	// C(n, 2) = n * (n - 1) / 2
	CSPProblem *problem =
			csp_problem_create(queen_count, queen_count * (queen_count - 1) / 2);
	for (size_t i = 0; i < queen_count; i++) {
		// Domain = number of possible row positions for a queen
		csp_problem_set_domain(problem, i, queen_count);
	}
	index = 0;
	// This way of creating the constraints ensures that each pair of queens
	// is always from different columns Otherwise we would need n * n
	// constraints to check all pairs of queens
	for (size_t i = 0; i < queen_count - 1; i++) {
		for (size_t j = i + 1; j < queen_count; j++) {
			// arity is 2 because we are checking compatibility between two
			// queens
			csp_problem_set_constraint(
					problem, index,
					csp_constraint_create(2, (CSPChecker *)queen_compatibles));
			csp_constraint_set_variable(csp_problem_get_constraint(problem, index), 0,
																	i);
			csp_constraint_set_variable(csp_problem_get_constraint(problem, index), 1,
																	j);
			index++;
		}
	}

	bool result;
	if (test_node_counter) {
		size_t *backtrack_counter = malloc(sizeof(size_t));

		result = csp_problem_solve(problem, queens, NULL, solve_type,
															 queens_checklist, NULL, backtrack_counter);

		assert(*backtrack_counter > 0);
		free(backtrack_counter);
	} else {
		result = csp_problem_solve(problem, queens, NULL, solve_type,
															 queens_checklist, NULL, NULL);
	}

	// Destroy the CSP problem
	while (index--) {
		csp_constraint_destroy(csp_problem_get_constraint(problem, index));
	}
	csp_problem_destroy(problem);

	// Print the solution
	if (result) {
		print_queens_solution(queen_count, queens);
	} else {
		printf("No solution found for %lu queens", queen_count);
	}

	// Free the queens array
	free(queens);

	return result;
}

int test_solver_solver_nqueens_tests(void) {
	csp_init();
	{
		SolveType basic = 0;
		SolveType fc = FC;
		SolveType basicMRV = MRV;
		SolveType fcMRV = FC | MRV;
		for (int i = 4; i < 10; i++) {
			assert(solve_queens(i, basic, false));
			assert(solve_queens(i, fc, false));
			assert(solve_queens(i, basicMRV, false));
			assert(solve_queens(i, fcMRV, false));
		}

		assert(solve_queens(11, fcMRV, true));
	}
	csp_finish();
	return EXIT_SUCCESS;
}
