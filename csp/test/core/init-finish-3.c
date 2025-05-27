/**
 * @file init-finish-3.h
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

int test_core_init_finish_3(void){
	assert(csp_init());
	assert(csp_init());
	assert(csp_init());
	assert(csp_finish());
	assert(csp_finish());
	assert(csp_finish());
	assert(csp_finish() == false);

	return EXIT_SUCCESS;
}
