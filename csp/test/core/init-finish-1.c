/**
 * @file init-finish-1.h
 *
 * @author Ch. Demko
 * @date 2024
 */

#ifdef NDEBUG
	#undef NDEBUG
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "csp.h"

int test_core_init_finish_1(void){
	assert(csp_initialised() == false);
	assert(csp_init());
	assert(csp_initialised());
	assert(csp_finish());
	assert(csp_initialised() == false);

	return EXIT_SUCCESS;
}