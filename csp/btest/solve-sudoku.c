/**
 * @file solve-sudoku.c
 * Sudoku solver using the CSP library to perform benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "csp.h"

// PRIVATE
static size_t backtrackCount = 0;

// Types defs
/**
 * Store the characteristics of the unknowns for the construction of the
 * CSPProblem
 */
typedef struct {
	size_t index;
	size_t x;
	size_t y;
} Unknown;

// Getters
bool sudoku_check(const CSPConstraint *constraint,
	const size_t *values, const void *data
){
	size_t* grid = (size_t*) data;

	const size_t arity = csp_constraint_get_arity(constraint);
	// `uIdx` is the coordinate of the unknown cell itself in the unknown list.
	const size_t uIdx = csp_constraint_get_variable(constraint, arity - 1);
	/** `cIdx` is the coordinate of a cell in the grid that is in the row,
	 * column or box of the unknown cell if it is <81; if it is >=81, it
	 * corresponds to it's index with values.
	 */
	size_t cIdx;

	for (size_t i = 0; i < arity - 1; i++){ // TODO: Use a while loop.
		cIdx = csp_constraint_get_variable(constraint, i);

		if(cIdx < 81){
			if(grid[cIdx] - 1 == values[uIdx])
				return false;
		}else if(values[cIdx - 81] == values[uIdx])
			return false;
	}

	return true;
}

void sudoku_getChecklist(const CSPProblem *csp,
	CSPConstraint** checklist, size_t* amount, const size_t index
){
	backtrackCount++; // TODO: Use yield syntax.

	*amount = 1;
	checklist[0] = csp_problem_get_constraint(csp, index);
}

// Functions
static void sudoku_print(const size_t* sudoku, const size_t* unknowns){
	printf("┌─────────┬─────────┬─────────┐\n");

	for(size_t row = 0, i = 0; row < 9; row++){
		printf("│");

		for(size_t col = 0; col < 9; col++){
			if(sudoku[row*9 + col] == 0){
				if(unknowns == NULL || unknowns[i] == 0) printf("   ");
				else printf(" %lu,", unknowns[i++]);
			}else printf(" %lu ", sudoku[row*9 + col]);

			if(col%3 == 2) printf("│");
		}

		printf("\n");
		if(row%3 == 2 && row != 8)
			printf("├─────────┼─────────┼─────────┤\n");
	}

	printf( "└─────────┴─────────┴─────────┘\n");
}

// PUBLIC
// Functions
int sudoku_solve(const size_t *sudoku, const char* resultFile, bool silent){
	size_t unknownCount = 0;
	size_t *unknowns;
	Unknown *unknownCells = malloc(sizeof(Unknown)*81);

	// Counts unknowns.
	for(size_t i = 0; i < 81; i++){
		if(sudoku[i] == 0){
			unknownCells[unknownCount] = (Unknown) {unknownCount, i%9, i/9};
			unknownCount++;
		}
	}

	CSPProblem* problem;

	unknownCells = realloc(unknownCells, sizeof(Unknown)*unknownCount);
	unknowns = malloc(sizeof(size_t)*unknownCount);

	// Initialise the library
	csp_init();
	{
		// Creates problem
		problem = csp_problem_create(unknownCount, unknownCount);

		for(size_t i = 0; i < csp_problem_get_num_domains(problem); i++)
			csp_problem_set_domain(problem, i, 9);

		// Creates constraints
		CSPConstraint* constraint;
		Unknown curUCell;
		size_t arity, variables[1 + 8 + 8 + 4];
		size_t bIdx;

		for(size_t uIdx = 0;
			uIdx < csp_problem_get_num_constraints(problem);
			uIdx++
		){
			curUCell = unknownCells[uIdx];
			arity = 0;

			for(size_t i = 0; i < uIdx; i++){
				if(
					( // box
						unknownCells[i].x/3 == curUCell.x/3
						&& unknownCells[i].y/3 == curUCell.y/3
					)
					// row
					|| unknownCells[i].x == curUCell.x
					// column
					|| unknownCells[i].y == curUCell.y
				){
					// Shifted by 81 to differentiate from grid cells.
					variables[arity++] = 81 + unknownCells[i].index;
				}
			}

			for(size_t row = 0; row < 9; row++){
				// Column
				if(sudoku[row*9 + curUCell.x] != 0)
					variables[arity++] = row*9 + curUCell.x;

				// Row
				if(sudoku[curUCell.y*9 + row] != 0)
					variables[arity++] = curUCell.y*9 + row;

				// TODO: Why ????????
			}

			for(size_t col = 0; col < 3; col++){
				for(size_t row = 0; row < 3; row++){
					bIdx = curUCell.x - curUCell.x%3
						+ (curUCell.y - curUCell.y%3)*9
						+ col + row*9;

					// Box: Not already added earlier through row or column.
					if(!(curUCell.x%3 == col || curUCell.y%3 == row)
						&& sudoku[bIdx] != 0
					){
						variables[arity++] = bIdx;
					}
				}
			}

			constraint = csp_constraint_create(arity + 1, &sudoku_check);
			csp_problem_set_constraint(problem, uIdx, constraint);

			for (size_t i = 0; i < arity; i++)
				csp_constraint_set_variable(constraint, i, variables[i]);

			// The last variable is the unknown cell itself, so the checker
			// function can use it for its logic
			csp_constraint_set_variable(constraint, arity, uIdx);
		}
	}

	// Solves a problem instance.
	{
		backtrackCount = 0;

		if(!silent)
			sudoku_print(sudoku, unknowns);

		clock_t st = clock();
		bool res = csp_problem_solve(problem, unknowns, sudoku,
			&sudoku_getChecklist
		);
		clock_t et = clock();

		float delay = ((float) (et - st))/CLOCKS_PER_SEC;

		if(!silent){
			printf("%s in %f!\n", res ? "SUCCEED" : "FAILED", delay);
			sudoku_print(sudoku, unknowns);
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

	free(unknowns);
	free(unknownCells);

	return EXIT_SUCCESS;
}