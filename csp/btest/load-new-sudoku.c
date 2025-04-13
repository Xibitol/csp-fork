/**
* @file load-new-sudoku.c
 * Sudoku generator using a backtracking algorithm to generate test grids used in benchmarking.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>

#define N 9

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

static void generate_sudoku(size_t grid[N][N]) {
    srand(time(0) ^ getpid() ^ (clock() << 16));
    fill_diagonal(grid);
    generator_solve_sudoku(grid, 0, 0);
}

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

size_t** load_new_sudoku(const size_t unknown_count, const size_t sudoku_count) {
    size_t** sudoku_list = malloc(sudoku_count * sizeof(size_t*));
    if (sudoku_list == NULL) {
        perror("malloc");
        return NULL;
    }
    for (size_t i = 0; i < sudoku_count; i++) {
        size_t* sudoku = calloc(81, sizeof(size_t));
        if (sudoku == NULL) {
            perror("calloc");
            free(sudoku_list);
            return NULL;
        }
        size_t grid[N][N] = {0};
        generate_sudoku(grid);
        remove_numbers(grid, unknown_count);
        for (size_t k = 0; k < N; k++) {
            for (size_t j = 0; j < N; j++) {
                if (grid[k][j] == 0) {
                    sudoku[k * N + j] = 9;
                } else {
                    sudoku[k * N + j] = grid[k][j] - 1;
                }
            }
        }
        sudoku_list[i] = sudoku;
    }
    return sudoku_list;
}