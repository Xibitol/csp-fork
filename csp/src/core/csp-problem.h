/**
 * @file csp-problem.h
 * Defines types and operations for a `CSPProblem`.
 *
 * @author Ch. Demko
 * @date 2024
 */

#pragma once

#if !defined (_CSP_H_INSIDE) && !defined (CSP_COMPILATION)
#error "Only <csp/csp.h> can be included directly."
#endif

#include <stdbool.h>
#include <stddef.h>

#include "csp-constraint.h"

// TYPE DEFINITIONS
/**
 * @brief The CSP problem.
 */
typedef struct _CSPProblem CSPProblem;

// CONSTRUCTORS
/**
 * @brief Create a CSP problem with the specified number of variables and
 * constraints.
 * @param num_domains The number of variables of the CSP problem.
 * @param num_constraints The number of constraints of the CSP problem.
 * @return The CSP problem created or NULL if an error occurred.
 * @pre The csp library is initialised.
 * @pre num_variables > 0
 * @pre num_constraints > 0
 * @post The CSP problem domains are initialised to 0.
 * @post The CSP problem constraints are initialised to NULL.
 * @post The CSP problem number of domains is set to the specified number of
 * domains.
 * @post The CSP problem number of constraints is set to the specified number of
 * constraints.
 */
extern CSPProblem *csp_problem_create(
	size_t num_domains, size_t num_constraints
);

// DESTRUCTORS
/**
 * @brief Destroy the CSP problem.
 * @param csp The CSP problem to destroy.
 * @pre The csp library is initialised.
 * @post The CSP problem constraints are freed.
 * @post The CSP problem domains is freed.
 * @post The CSP problem is freed.
 */
extern void csp_problem_destroy(CSPProblem *csp);

// GETTERS
/**
 * @brief Get the number of constraints of the CSP problem.
 * @param csp The CSP problem to get the number of constraints.
 * @return The number of constraints of the CSP problem.
 * @pre The csp library is initialised.
 */
extern size_t csp_problem_get_num_constraints(const CSPProblem *csp);
/**
 * @brief Get the constraint of the CSP problem at the specified index.
 * @param csp The CSP problem to get the constraint.
 * @param index The index of the constraint.
 * @return The constraint at the specified index.
 * @pre The csp library is initialised.
 */
extern CSPConstraint *csp_problem_get_constraint(const CSPProblem *csp,
	size_t index
);
/**
 * @brief Get the number of domains of the CSP problem.
 * @param csp The CSP problem to get the number of domains.
 * @return The number of domains of the CSP problem.
 * @pre The csp library is initialised.
 */
extern size_t csp_problem_get_num_domains(const CSPProblem *csp);
/**
 * @brief Get the domain of the CSP problem at the specified index.
 * @param csp The CSP problem to get the domain.
 * @param index The index of the domain.
 * @return The domain at the specified index.
 * @pre The csp library is initialised.
 * @pre index < csp->num_domains
 */
extern size_t csp_problem_get_domain(const CSPProblem *csp, size_t index);

// SETTERS
/**
 * @brief Set the constraint of the CSP problem at the specified index.
 * @param csp The CSP problem to set the constraint.
 * @param index The index of the constraint.
 * @param constraint The constraint to set.
 * @pre The csp library is initialised.
 * @pre index < csp->num_constraints
 * @pre constraint != NULL
 */
extern void csp_problem_set_constraint(CSPProblem *csp,
	size_t index, CSPConstraint *constraint
);
/**
 * @brief Set the domain of the CSP problem at the specified index.
 * @param csp The CSP problem to set the domain.
 * @param index The index of the domain.
 * @param domain The domain to set.
 * @pre The csp library is initialised.
 * @pre index < csp->num_domains
 */
extern void csp_problem_set_domain(CSPProblem *csp,
	size_t index, size_t domain
);