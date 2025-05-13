/**
 * @file solve-queens.c
 * N_Queens problem solver using the CSP library to perform benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "queens-solve.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "csp.h"
#include "util/unused.h"

// PRIVATE
static size_t backtrackCount = 0;

// Getters
// Checks if the queens are compatible.
static bool queens_are_comptatible(const CSPConstraint *constraint,
	const size_t *values, const void *UNUSED_VAR(data)
){
	size_t x0 = csp_constraint_get_variable(constraint, 0);
	size_t x1 = csp_constraint_get_variable(constraint, 1);

	size_t y0 = values[x0];
	size_t y1 = values[x1];

	return x0 != x1 && y0 != y1 && x0 + y1 != x1 + y0 && x0 + y0 != x1 + y1;
}

static void queens_get_checklist(const CSPProblem *csp,
	CSPConstraint** checklist, size_t* amount, const size_t index
){
	backtrackCount++; // TODO: Use yield syntax.

	*amount = index;
	size_t queenCount = csp_problem_get_num_domains(csp);

	// Because of the way the constraints are created, for 6 queens, the formula
	// below per index is equal to:
	// i: 0, 1,  2,  3,  4
	// -------------------
	// 1: 0  .,  .,  .,  .
	// 2: 1, 5   .,  .,  .
	// 3: 2, 6,  9,  .,  .
	// 4: 3, 7, 10, 12,  .
	// 5: 4, 8, 11, 13, 14
	for (size_t i = 0; i < index; i++)
		checklist[i] = csp_problem_get_constraint(csp,
			i*queenCount - i*(i + 1)/2 + (index - i - 1)
		);
}

// Functions
// Prints the solution
static void queens_print(unsigned int queenCount, const size_t *queens){
	printf("┌");
	for(size_t col = 0; col < queenCount - 1; col++) printf("───┬");
	printf("───┐\n");

	for(size_t row = 0; row < queenCount; row++){
		printf("│");
		for(size_t col = 0; col < queenCount; col++)
			printf(" %s │", queens[row] == col ? "♛" : " ");
		printf("\n");

		if(row != queenCount - 1){
			printf("├");
			for(size_t col = 0; col < queenCount - 1; col++) printf("───┼");
			printf("───┤\n");
		}
	}

	printf("└");
	for(size_t col = 0; col < queenCount - 1; col++) printf("───┴");
	printf("───┘\n");
}

// PUBLIC
// Functions
int queens_solve(size_t queenCount, const char* resultFile, bool silent) {
	size_t *queens = calloc(queenCount, sizeof(size_t));

	if(queens == NULL){
		perror("calloc failed");
		return EXIT_FAILURE;
	}

	CSPProblem* problem;

	csp_init();
	{
		size_t index;

		// Creates problem
		/* num_domains is the number of queens here, also the number of columns
		 * since each queen is in a different column. Each constraint
		 * corresponds to a pair of queens that need to be checked for
		 * compatibility. The number of constraints corresponds to the number of
		 * pairs of queens that need to be checked.
		 * This is equal to the combination C(n, 2) = n*(n - 1) / 2
		 */
		problem = csp_problem_create(queenCount, queenCount*(queenCount - 1)/2);

		// Domain = number of possible row positions for a queen
		for (size_t i = 0; i < queenCount; i++)
			csp_problem_set_domain(problem, i, queenCount);

		// Creates constraints.
		/* This way of creating the constraints ensures that each pair of queens
		 * is always from different columns. Otherwise we would need n * n
		 * constraints to check all pairs of queens.
		 */
		index = 0;
		for (size_t i = 0; i < queenCount - 1; i++){
			for (size_t j = i + 1; j < queenCount; j++){
				// arity is 2 because we are checking compatibility between two
				// queens.
				csp_problem_set_constraint(problem, index,
					csp_constraint_create(2, &queens_are_comptatible)
				);

				csp_constraint_set_variable(
					csp_problem_get_constraint(problem, index), 0, i
				);
				csp_constraint_set_variable(
					csp_problem_get_constraint(problem, index), 1, j
				);

				index++;
			}
		}
	}

	// Solves a problem instance.
	{
		clock_t st = clock();
		bool res = csp_problem_solve(problem, queens, NULL,
			&queens_get_checklist
		);
		clock_t et = clock();

		float delay = ((float) (et - st))/CLOCKS_PER_SEC;

		if(!silent){
			printf("%s in %f!\n", res ? "SUCCEED" : "FAILED", delay);
			queens_print(queenCount, queens);
			printf("Backtrack counts: %zu.\n", backtrackCount);
		}

		FILE* file = fopen(resultFile, "a");
		fprintf(file, "%f %zu\n", delay, backtrackCount);
		fclose(file);
	}

	// Destroys constraints and problem.
	{
		for(size_t i = 0; i < csp_problem_get_num_constraints(problem); i++)
			csp_constraint_destroy(csp_problem_get_constraint(problem, i));

		csp_problem_destroy(problem);
	}
	csp_finish();

	free(queens);

	return EXIT_SUCCESS;
}
