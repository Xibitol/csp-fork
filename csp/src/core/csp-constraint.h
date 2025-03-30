/**
 * @file csp-constraint.h
 * Defines types and operations for a `CSPConstraint`.
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

// TYPE DEFINITIONS
/**
 * @brief The constraint of a CSP problem.
 */
typedef struct _CSPConstraint CSPConstraint;

/**
 * @brief The check function of a CSP constraint.
 * @param constraint The constraint to check.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @return true if the constraint is satisfied, false otherwise.
 * @pre constraint != NULL
 * @pre values != NULL
 */
typedef bool CSPChecker(const CSPConstraint *, const size_t *, const void *);

// CONSTRUCTORS
/**
 * @brief Create a constraint with the specified arity and check function.
 * @param arity The arity of the constraint.
 * @param check The check function of the constraint.
 * @return The constraint created or NULL if an error occurred.
 * @pre The csp library is initialised.
 * @pre arity > 0
 * @pre check != NULL
 * @post The constraint variables are initialised to 0.
 * @post The constraint arity is set to the specified arity.
 * @post The constraint check function is set to the specified check function.
 */
extern CSPConstraint *csp_constraint_create(size_t arity, CSPChecker *check);

// DESTRUCTORS
/**
 * @brief Destroy the constraint.
 * @param constraint The constraint to destroy.
 * @pre The csp library is initialised.
 * @pre constraint != NULL
 * @post The constraint variables are freed.
 * @post The constraint is freed.
 */
extern void csp_constraint_destroy(CSPConstraint *constraint);

// GETTERS
/**
 * @brief Get the arity of the constraint.
 * @param constraint The constraint to get the arity.
 * @return The arity of the constraint.
 * @pre The csp library is initialised.
 */
extern size_t csp_constraint_get_arity(const CSPConstraint *constraint);
/**
 * @brief Get the check function of the constraint.
 * @param constraint The constraint to get the check function.
 * @return The check function of the constraint.
 * @pre The csp library is initialised.
 */
extern CSPChecker *csp_constraint_get_check(const CSPConstraint *constraint);
/**
 * @brief Get the variable of the constraint at the specified index.
 * @param constraint The constraint to get the variable.
 * @param index The index of the variable.
 * @return The variable at the specified index.
 * @pre The csp library is initialised.
 * @pre index < constraint->arity
 */
extern size_t csp_constraint_get_variable(const CSPConstraint *constraint,
	size_t index
);

// SETTERS
/**
 * @brief Set the variable of the constraint at the specified index.
 * @param constraint The constraint to set the variable.
 * @param index The index of the variable.
 * @param variable The variable to set.
 * @pre The csp library is initialised.
 * @pre index < constraint->arity
 */
extern void csp_constraint_set_variable(CSPConstraint *constraint,
	size_t index, size_t variable
);