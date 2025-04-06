//
// Created by adrien on 28/03/25.
//


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "load-new-sudoku.h"
#include "solve-sudoku.h"

int sudoku_benchmark(int average_amount, int unknown_increment) {
    FILE *file1 = fopen("sudoku_benchmark.txt", "w");
    if (file1 == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fclose(file1);

    size_t** sudokus = 0;
    for (int i = 5; i < 81; i+=unknown_increment) {
        sudokus = load_new_sudoku(i, average_amount);
        for (int j = 0; j < average_amount; j++) {
            solve_sudoku(sudokus[j]);
        }

    }
    free(sudokus);
    return EXIT_SUCCESS;
}