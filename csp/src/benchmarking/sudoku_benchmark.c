//
// Created by adrien on 28/03/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int average_amount = atoi(argv[1]);

    FILE *file1 = fopen("sudoku_benchmark.txt", "w");
    if (file1 == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fclose(file1);

    for (int i = 5; i < 81; i+=5) {
        pid_t pid = fork();
        switch (pid){
            case -1:
                perror("fork failed");
                return EXIT_FAILURE;
            case 0:
                char n_unknown[10];
                snprintf(n_unknown, sizeof(n_unknown), "%d", i);
                char n_sudoku[10];
                snprintf(n_sudoku, sizeof(n_sudoku), "%d", average_amount);
                execl("./src/executables/load-new-sudoku", "./src/executables/load-new-sudoku", n_unknown, n_sudoku, (char *)NULL);
                perror("execl");
                return EXIT_FAILURE;
            default:
        }
        wait(NULL);

        FILE *file3 = fopen("sudoku.txt", "r");
        if (file3 == NULL) {
            perror("fopen");
            return EXIT_FAILURE;
        }

        char sudoku[83]; // 81 characters + 1 for null terminator
        for (int j = 0; j < average_amount; j++) {

            if (fgets(sudoku, sizeof(sudoku), file3) == NULL) {
                perror("fgets");
                return EXIT_FAILURE;
            }

            pid_t pid2 = fork();
            switch (pid2){
                case -1:
                    perror("fork failed");
                    return EXIT_FAILURE;
                case 0:
                    execl("./src/executables/solve-sudoku", "./src/executables/solve-sudoku", sudoku, (char *)NULL);
                    perror("execl");
                    return EXIT_FAILURE;
                default:
            }
            wait(NULL);
        }
    }
    return EXIT_SUCCESS;
}