/**
 * @file domain-tests.c
 * Tests for solver/domains.c
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
#include <stdlib.h>

#include "csp.h"

int test_solver_domain_tests(void) {
	csp_init();
	{
		Domain* domain = domain_create(4);
    Domain* domains[] = {domain};
		assert(domain != NULL);
		assert(domain_get_amount(domain) == 4);

    size_t stack_top = 0;
    size_t stop_point = 0;
    DomainChange* stack = domain_change_stack_create(4);
    assert(stack != NULL);

		for (size_t i = 0; i < 4; i++) {
			assert(domain_get_value(domain, i) == i);
		}
    domain_change_stack_add(stack, &stack_top, 0, 2);
		domain_remove_value(domain, 2);
		assert(domain_get_value(domain, 1) == 1);
		assert(domain_get_value(domain, 2) == 3);
		assert(domain_get_amount(domain) == 3);
    domain_change_stack_restore(stack, &stack_top, &stop_point, domains);
    assert(domain_get_amount(domain) == 4);
    assert(domain_get_value(domain, 3) == 2);
    assert(domain_get_value(domain, 2) == 3);

		domain_destroy(domain);
    domain_change_stack_destroy(stack);
	}
	csp_finish();
	return EXIT_SUCCESS;
}
