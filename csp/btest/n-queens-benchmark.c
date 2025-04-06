//
// Created by adrien on 28/03/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "solve-queens.h"

int n_queens_benchmark(int test_count){

    FILE *file = fopen("n_queens_benchmark.txt", "w");
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fclose(file);

    for (int i = 4; i < test_count+4; i++) {
        solve_queens(i);
    }
    return EXIT_SUCCESS;
}