/**
 * @file csp-problem.inc.h
 *
 * @author Ch. Demko
 * @date 2024
 */

#pragma once

#include <stddef.h>

#include "csp-constraint.h"

/**
 * @brief The CSP problem.
 * @param num_domains The number of variables.
 * @param domains The domains of the variables.
 * @param num_constraints The number of constraints.
 * @param constraints The constraints of the problem.
 */
struct _CSPProblem {
	size_t num_domains;
	size_t *domains;
	size_t num_constraints;
	CSPConstraint **constraints;
};
