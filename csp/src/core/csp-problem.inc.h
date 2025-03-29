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
 * @var num_domains The number of variables.
 * @var domains The domains of the variables.
 * @var num_constraints The number of constraints.
 * @var constraints The constraints of the problem.
 */
struct _CSPProblem {
	size_t num_domains;
	size_t *domains;
	size_t num_constraints;
	CSPConstraint **constraints;
  };