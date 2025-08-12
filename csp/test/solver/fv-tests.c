/**
 * @file fv-tests.c
 * Tests for solver/filled-variables.c
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "csp.h"

int test_solver_fv_tests(void) {
	csp_init();
	{
		FilledVariables* fv = filled_variables_create(9);
		assert(fv != NULL);

		assert(filled_variables_get_size(fv) == 9);

		assert(filled_variables_next_unfilled(fv, 0) == 0);
		assert(filled_variables_next_filled(fv, 0) == SIZE_MAX);
		filled_variables_mark_filled(fv, 2);
		assert(!filled_variables_all_filled(fv));
		assert(filled_variables_is_filled(fv, 2));
		assert(filled_variables_next_filled(fv, 0) == 2);
		assert(filled_variables_next_unfilled(fv, 2) == 3);
		filled_variables_mark_unfilled(fv, 2);
		assert(!filled_variables_is_filled(fv, 2));

		for (int i = 0; i < 9; i++) {
			filled_variables_mark_filled(fv, i);
		}
		assert(filled_variables_all_filled(fv));
		assert(filled_variables_next_unfilled(fv, 5) == SIZE_MAX);
		assert(filled_variables_next_filled(fv, 5) == 5);

		FilledVariables* fv2 = filled_variables_create(3);
		assert(filled_variables_next_filled(fv2, 2) == SIZE_MAX);

		filled_variables_destroy(fv);
		filled_variables_destroy(fv2);
	}
	csp_finish();
	return EXIT_SUCCESS;
}
