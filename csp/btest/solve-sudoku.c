/**
 * @file solve-sudoku.c
 * Sudoku solver using the CSP library to perform benchmarking.
 *
 * @author Xibitol <xibitol@pimous.dev>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "solve-sudoku.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "csp.h"
#include "csp-btest.h"

typedef struct{
	size_t* sudoku;
	size_t* unknownCells;
} SudokuData;

static size_t constraintsChecked[3] = {0};

/** Row musn't have repeated values. */
static bool rowChecker(const CSPConstraint *constraint,
	const size_t *value, const void *data
){
	SudokuData *sd = (SudokuData*) data;
	size_t row = sd->unknownCells[csp_constraint_get_variable(constraint, 0)]/9;
	size_t valueCounts[9] = {0};

	size_t col = 0, i = 0, cIdx, val;
	do{
		cIdx = row*9 + col;

		if(sd->sudoku[cIdx] != 0) val = sd->sudoku[cIdx];
		else val = value[csp_constraint_get_variable(constraint, i++)] + 1;

		valueCounts[val - 1]++;
	}while(valueCounts[val - 1] <= 1 && ++col < 9);

	constraintsChecked[0]++;

	return col == 9;
}
/** Column sum must be equal to 45. */
static bool colChecker(const CSPConstraint *constraint,
	const size_t *value, const void *data
){
	SudokuData *sd = (SudokuData*) data;
	size_t col = sd->unknownCells[csp_constraint_get_variable(constraint, 0)]%9;
	size_t sum = 0;

	size_t cIdx;
	for(size_t row = 0, i = 0; row < 9; row++){
		cIdx = row*9 + col;

		if(sd->sudoku[cIdx] != 0) sum += sd->sudoku[cIdx];
		else sum += value[csp_constraint_get_variable(constraint, i++)] + 1;
	}

	constraintsChecked[1]++;

	return sum == 45;
}
/** Box sum must be equal to 45. */
static bool blockChecker(const CSPConstraint *constraint,
	const size_t *value, const void *data
){
	SudokuData *sd = (SudokuData*) data;
	size_t var0Idx = csp_constraint_get_variable(constraint, 0);
	size_t block = (sd->unknownCells[var0Idx]/(9*3))*3
		+ (sd->unknownCells[var0Idx]/3)%3;
	size_t sum = 0;

	size_t bIdx = (block/3)*(9*3) + (block%3)*3, cIdx = 0;
	for(size_t cell = 0, i = 0; cell < 9; cell++){
		cIdx = bIdx + (cell/3)*9 + cell%3;

		if(sd->sudoku[cIdx] != 0) sum += sd->sudoku[cIdx];
		else sum += value[csp_constraint_get_variable(constraint, i++)] + 1;
	}

	constraintsChecked[2]++;

	return sum == 45;
}

/** @author agueguen-LR */
static void print_sudoku_solution(const size_t* sudoku, const size_t* unknowns){
	printf("┌─────────┬─────────┬─────────┐\n");

	for(size_t row = 0, i = 0; row < 9; row++){
		printf("│");

		for(size_t col = 0; col < 9; col++){
			if(sudoku[row*9 + col] == 0){
				if(unknowns == NULL || unknowns[i] + 1 == 0) printf("   ");
				else printf(" %lu,", unknowns[i++] + 1);
			}else printf(" %lu ", sudoku[row*9 + col]);

			if(col % 3 == 2) printf("│");
		}

		printf("\n");
		if(row%3 == 2 && row != 8)
			printf("├─────────┼─────────┼─────────┤\n");
	}

	printf( "└─────────┴─────────┴─────────┘\n");
}

int solve_sudoku(size_t *sudoku, bool silent){
	size_t unknownCount = 0;
	size_t *unknowns = NULL, *unknownCells = malloc(sizeof(size_t)*81);

	// Changes values and count unknowns.
	for(size_t i = 0; i < 81; i++){
		sudoku[i] = sudoku[i] == 9 ? 0 : sudoku[i] + 1;

		if(sudoku[i] == 0){
			unknownCells[unknownCount] = i;
			unknownCount++;
		}
	}

	// Prints sudoku.
	if(!silent) print_sudoku_solution(sudoku, unknowns);

	CSPProblem* problem;

	unknownCells = realloc(unknownCells, sizeof(size_t)*unknownCount);
	unknowns = malloc(sizeof(size_t)*unknownCount);

	SudokuData sudokuData = (SudokuData) {sudoku, unknownCells};

	csp_init();
	{
		// Creates constraints.
		size_t consIdx = 0;
		CSPConstraint* constraints[9 + 9 + 9] = {NULL};
		size_t unkIdx = 0;
		size_t arity, variables[9] = {0};
		size_t bIdx, cIdx;

		for(size_t row = 0; row < 9; row++){
			arity = 0;
			for(size_t col = 0, i = 0; col < 9; col++){
				cIdx = row*9 + col;

				if(sudoku[cIdx] == 0){
					arity++;
					variables[i++] = unkIdx++;
				}
			}

			if(arity > 0){
				constraints[consIdx] = csp_constraint_create(arity,
					&rowChecker
				);
				for(size_t i = 0; i < arity; i++)
					csp_constraint_set_variable(constraints[consIdx],
						i, variables[i]
					);

				consIdx++;
			}
		}

		for(size_t col = 0; col < 9; col++){
			arity = 0;
			for(size_t row = 0, i = 0; row < 9; row++){
				cIdx = row*9 + col;

				if(sudoku[cIdx] == 0){
					arity++;
					variables[i++] = cIdx;
				}
			}

			if(arity > 0){
				constraints[consIdx] = csp_constraint_create(arity,
					&colChecker
				);

				unkIdx = 0;
				for(size_t i = 0; i < arity; unkIdx++)
					if(unknownCells[unkIdx] == variables[i])
						csp_constraint_set_variable(constraints[consIdx],
							i++, unkIdx
						);

				consIdx++;
			}
		}

		for(size_t block = 0; block < 9; block++){
			arity = 0;
			bIdx = (block/3)*(9*3) + (block%3)*3;
			for(size_t cell = 0, i = 0; cell < 9; cell++){
				cIdx = bIdx + (cell/3)*9 + cell%3;

				if(sudoku[cIdx] == 0){
					arity++;
					variables[i++] = cIdx;
				}
			}

			if(arity > 0){
				constraints[consIdx] = csp_constraint_create(arity,
					&blockChecker
				);

				unkIdx = 0;
				for(size_t i = 0; i < arity; unkIdx++)
					if(unknownCells[unkIdx] == variables[i])
						csp_constraint_set_variable(constraints[consIdx],
							i++, unkIdx
						);

				consIdx++;
			}
		}

		// Creates problem
		problem = csp_problem_create(unknownCount, consIdx);

		for(size_t i = 0; i < csp_problem_get_num_domains(problem); i++)
			csp_problem_set_domain(problem, i, 9);
		for(size_t i = 0; i < csp_problem_get_num_constraints(problem); i++)
			csp_problem_set_constraint(problem, i, constraints[i]);
	}

	// Solves a problem instance.
	{
		FILE* file = fopen(SUDOKU_RESULT_FILE, "a");

		clock_t st = clock();
		bool res = csp_problem_solve(problem, unknowns, &sudokuData, NULL);
		clock_t et = clock();

		float delay = ((float) (et - st))/CLOCKS_PER_SEC;

		printf("%s in %f!\n", res ? "SUCCEED" : "FAILED", delay);
		printf("Row checks: %lu, Column checks: %lu and Block checks: %lu.\n",
			constraintsChecked[0], constraintsChecked[1], constraintsChecked[2]
		);
		fprintf(file, "%f %zu\n", delay, (size_t) 0);

		fclose(file);
	}

	// Destroys constraints and problem.
	{
		for(size_t i = 0; i < csp_problem_get_num_constraints(problem); i++)
			csp_constraint_destroy(csp_problem_get_constraint(problem, i));

		csp_problem_destroy(problem);
	}
	csp_finish();

	// Prints resolved sudoku
	if(!silent) print_sudoku_solution(sudoku, unknowns);

	free(unknowns);
	free(unknownCells);

	return EXIT_SUCCESS;
}
