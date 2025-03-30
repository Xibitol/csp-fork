//
// Created by adrien on 28/03/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(void){

    FILE *file = fopen("n_queens_benchmark.txt", "w");
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fclose(file);

    for (int i = 4; i < 22; i++) {
        pid_t pid = fork();
        switch (pid){
            case -1:
                perror("fork failed");
                return EXIT_FAILURE;
            case 0:
                char n_str[10];
                snprintf(n_str, sizeof(n_str), "%d", i);
                execl("./src/executables/solve-queens", "./src/executables/solve-queens", n_str, (char *)NULL);
                perror("execl");
                return EXIT_FAILURE;
            default:
        }
        wait(NULL);
    }
    return EXIT_SUCCESS;
}