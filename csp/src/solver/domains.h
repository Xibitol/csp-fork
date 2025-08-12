/**
 * @file domains.h
 * Functions to manipulate domain-related structures
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#if !defined(_CSP_H_INSIDE) && !defined(CSP_COMPILATION)
#error "Only <csp/csp.h> can be included directly."
#endif

#include <stddef.h>

#include "core/csp-constraint.h"
#include "core/csp-problem.h"
#include "solver/filled-variables.h"

/**
 * The Domain structure
 */
typedef struct _Domain Domain;

/**
 * The DomainChange stack structure
 */
typedef struct _DomainChange DomainChange;

/**
 * Create a new Domain structure.
 * @param size The size of the domain.
 * @return A pointer to the new Domain structure, or NULL on failure.
 */
extern Domain* domain_create(size_t size);

/**
 * Free the memory allocated for a Domain structure.
 * @param domain The Domain structure to free.
 */
extern void domain_destroy(Domain* domain);

/**
 * Get the value at the specified index
 * @param domain The Domain structure
 * @param index The index of the value to get in the domain
 * @return the value at the specified index in the domain
 */
extern size_t domain_get_value(Domain* domain, size_t index);

/**
 * Get the amount of values in the domain
 * @param domain The Domain structure
 * @return The amount of values in the domain
 */
extern size_t domain_get_amount(Domain* domain);

/**
 * Remove a value from the domain
 * ! This also reduces 'amount' and shifts all the next values
 * by reducing their index by one
 *
 * @param domain The Domain structure
 * @param index The index of the element to remove
 */
extern void domain_remove_value(Domain* domain, size_t index);

/**
 * Print the values in a Domain structure.
 * @param domain The Domain structure to print.
 */
extern void print_domain(const Domain* domain);

/**
 * Print the values in an array of Domain structures.
 * @param domains The array of Domain structures to print.
 * @param num_domains The number of Domain structures in the array.
 */
extern void print_domains(const Domain** domains, const size_t num_domains);

/**
 * Create a new DomainChange structure.
 * @param size The size of the change stack.
 * @return A pointer to the new DomainChange structure, or NULL on failure.
 */
extern DomainChange* domain_change_stack_create(size_t size);

/**
 * Free the memory allocated for a DomainChange structure.
 * @param stack The DomainChange structure to free.
 */
extern void domain_change_stack_destroy(DomainChange* stack);

/**
 * Restore the domains from the change stack up to the specified stop point.
 * @param stack The DomainChange structure.
 * @param stack_top Pointer to the top of the stack.
 * @param stop_point Pointer to the point to restore up to.
 * @param domains The array of domains to restore.
 */
extern void domain_change_stack_restore(const DomainChange* stack,
																				size_t* stack_top,
																				const size_t* stop_point,
																				Domain** domains);

/**
 * Add a change to the change stack.
 * @param stack The DomainChange structure.
 * @param stack_top Pointer to the top of the stack.
 * @param domain_index The index of the domain that changed.
 * @param value The value that was removed from the domain.
 */
extern void domain_change_stack_add(DomainChange* stack, size_t* stack_top,
																		size_t domain_index, size_t value);
