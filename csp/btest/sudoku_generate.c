/**
 * @file sudoku_generate.c
 * Sudoku generator using a backtracking algorithm to generate test grids used
 * in benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "sudoku_generate.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

// PRIVATE
#define N 9

// Getters
/**
 * Checks if a number can be placed in the given cell of the Sudoku grid.
 * @param grid The Sudoku grid.
 * @param row The row index.
 * @param col The column index.
 * @param num The number to check.
 * @return true if the number can be placed, false otherwise.
 */
static bool isSafe(size_t grid[N][N], size_t row, size_t col, size_t num){
	for(size_t x = 0; x < N; x++) // TODO: Use a while loop.
		if(grid[row][x] == num || grid[x][col] == num)
			return false;

	size_t startRow = row - row%3, startCol = col - col%3;
	for(size_t i = 0; i < 3; i++) // TODO: Use a while loop.
		for(size_t j = 0; j < 3; j++)
			if(grid[i + startRow][j + startCol] == num)
				return false;

	return true;
}

// Setters
/**
 * Fills the diagonal boxes of the Sudoku grid with random numbers.
 * @param grid The Sudoku grid to fill.
 */
static void sudoku_fillDiagonal(size_t grid[N][N]){
	for(size_t i = 0; i < N; i += 3){ // FIXME: Too many loops.
		for(size_t j = 0; j < 3; j++){
			for(size_t k = 0; k < 3; k++){
				size_t num;

				do{
					num = rand()%9 + 1;
				}while(!isSafe(grid, i + j, i + k, num));

				grid[i + j][i + k] = num;
			}
		}
	}
}
/**
 * Solves the Sudoku grid using backtracking.
 * @param grid The Sudoku grid to solve.
 * @param row The current row to fill.
 * @param col The current column to fill.
 * @return 1 if the Sudoku is solved, 0 otherwise.
 */
static int sudoku_solve(size_t grid[N][N], size_t row, size_t col){
	if(row == N - 1 && col == N) return 1;
	else if(col == N){
		row++;
		col = 0;
	}

	if(grid[row][col] != 0)
		return sudoku_solve(grid, row, col + 1);

	for(size_t num = 1; num <= N; num++){
		if(isSafe(grid, row, col, num)){
			grid[row][col] = num;

			if(sudoku_solve(grid, row, col + 1)) return 1;
		}

		grid[row][col] = 0;
	}

	return 0;
}
/**
 * Removes random numbers from the Sudoku grid to create a puzzle.
 * @param grid The Sudoku grid to modify.
 * @param amount The number of cells to remove.
 */
static void sudoku_removeNumbers(size_t grid[N][N], size_t amount){
	size_t count = 0;

	while(count < amount){
		size_t row = rand()%N;
		size_t col = rand()%N;

		if(grid[row][col] != 0){
			grid[row][col] = 0;
			count++;
		}
	}
}

// Functions
/**
 * Generates a solved Sudoku in a grid. It places random numbers in the diagonal
 * boxes and performs #solve_sudoku.
 * @param grid The Sudoku grid to fill.
 */
static void sudoku_generateSolved(size_t grid[N][N]){
	srand(time(0)^getpid()^(clock() << 16));

	sudoku_fillDiagonal(grid);
	sudoku_solve(grid, 0, 0);
}

// PUBLIC
// Functions
/**
 * Generates a new Sudoku puzzle with a specified number of unknown cells.
 * @param count number of Sudoku grids to generate.
 * @param unknownCount amount of unknown cells in the Sudoku grid.
 * @return A pointer to an array of Sudoku grids, or NULL if an error occurred.
 */
size_t** sudoku_generate(
	const size_t count, const size_t unknownCount
){
	size_t ** sudokus;
	size_t* data;

	if((sudokus = malloc(count*__SIZEOF_POINTER__)) == NULL){
		perror("calloc");
		return NULL;
	}

	if((data = malloc(count*N*N*sizeof(size_t))) == NULL){
		perror("calloc");
		free(sudokus);
		return NULL;
	}

	for(size_t i = 0; i < count; i++){
		size_t grid[N][N] = {0};

		sudoku_generateSolved(grid);
		sudoku_removeNumbers(grid, unknownCount);

		sudokus[i] = &data[N*N*i];
		for(size_t k = 0; k < N; k++)
			for(size_t j = 0; j < N; j++)
				sudokus[i][k*N + j] = grid[k][j];
	}

	return sudokus;
}