/**
 * @file csp-btest.h
 * Entry point of CSP lib's benchmarking program.
 *
 * @author Xibitol <xibitol@pimous.dev>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include <stdlib.h>
#include <stdio.h>

#include "./sudoku-benchmark.h"
#include "./n-queens-benchmark.h"

int main(void){
	sudoku_benchmark(5, 5);
	n_queens_benchmark(16);
	return EXIT_SUCCESS;
}
