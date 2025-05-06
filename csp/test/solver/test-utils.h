#pragma once
#include "csp.h"


static bool constraint_equal(const CSPConstraint *c, const size_t *v, const void *d) {
    (void)d;
    return v[csp_constraint_get_variable(c, 0)] == v[csp_constraint_get_variable(c, 1)];
}


static bool constraint_diff(const CSPConstraint *c, const size_t *v, const void *d) {
    (void)d;
    return v[csp_constraint_get_variable(c, 0)] != v[csp_constraint_get_variable(c, 1)];
}