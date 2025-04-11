/**
 * @file csp-constraint.inc.h
 *
 * @author Ch. Demko
 * @date 2024
 */

#pragma once

#include <stddef.h>

#include "csp-constraint.h"

/**
 * @brief The constraint of a CSP problem.
 * @var check The check function of the constraint.
 * @var arity The arity of the constraint.
 * @var variables The variables of the constraint.
 */
struct _CSPConstraint {
  CSPChecker *check;
  size_t arity;
  size_t variables[];
};