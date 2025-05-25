/**
 * @file types-and-structs.h
 * File containing the definition of the types and structures used in the CSP
 * And the functions to manipulate them.
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
#include <stdint.h>

#include "core/csp-constraint.h"
#include "core/csp-problem.h"

typedef enum {
	FC = 1,
	OVARS_MIN = 2,
	OVARS_MAX = 4,
	OVALS = 8,
} SolveType;

/**
 * Structure to represent the domain of a variable in a CSP problem.
 * It contains the number of values in the domain and an array of values.
 */
typedef struct {
	size_t amount;
	size_t values[];
} Domain;

/**
 * Structure to track changes in the domain of a variable during forward
 * checking. Use as a stack to store the changes.
 * It stores the index of the domain and the value that was removed.
 */
typedef struct {
	size_t domain_index;
	size_t value;
} DomainChange;

/**
 * Structure to track filled variables in a CSP problem.
 * It uses a bitset to efficiently track which variables are filled.
 */
typedef struct {
	size_t size;			// Number of variables
	uint8_t* bitset;	// Bitset to track filled variables
} FilledVariables;

/**
 * Get the list of value constraints to verify for the current variable to know
 * if the CSPProblem is consistent.
 * @note This function is used by #csp_problem_is_consistent.
 * @param csp The CSP problem.
 * @param checklist Array to store the list of constraints to verify.
 * @param amount Pointer to size_t to store the number of constraints to verify.
 * @param index The index of the current variable.
 * @param fv The FilledVariables structure to track filled variables.
 */
typedef void CSPValueChecklist(const CSPProblem* csp, CSPConstraint** checklist,
	size_t* amount, size_t index, FilledVariables* fv
);

/**
 * Get the list of data constraints to verify for the current variable to know
 * if the CSPProblem is consistent.
 * @param csp The CSP problem.
 * @param checklist Array to store the list of constraints to verify.
 * @param amount Pointer to size_t to store the number of constraints to verify.
 * @param index The index of the current variable.
 */
typedef void CSPDataChecklist(const CSPProblem* csp, CSPConstraint** checklist,
	size_t* amount, size_t index
);

/**
 * Mark a variable as filled.
 * @param fv The FilledVariables structure.
 * @param index The index of the variable to mark as filled.
 */
extern void filled_variables_mark_filled(FilledVariables* fv, size_t index);

/**
 * Check if a variable is filled.
 * @param fv The FilledVariables structure.
 * @param index The index of the variable to check.
 * @return true if the variable is filled, false otherwise.
 */
extern bool filled_variables_is_filled(const FilledVariables* fv, size_t index);

/**
 * Mark a variable as unfilled.
 * @param fv The FilledVariables structure.
 * @param index The index of the variable to mark as unfilled.
 */
extern void filled_variables_mark_unfilled(FilledVariables* fv, size_t index);

/**
 * Check if all variables are filled.
 * @param fv The FilledVariables structure.
 * @return true if all variables are filled, false otherwise.
 */
extern bool filled_variables_all_filled(const FilledVariables* fv);

/**
 * Get the next unfilled variable.
 * @param fv The FilledVariables structure.
 * @param index The index to start searching from.
 * @return The index of the next unfilled variable, or SIZE_MAX if all are
 * filled.
 */
extern size_t filled_variables_next_unfilled(const FilledVariables* fv,
	size_t index
);

/**
 * Get the next filled variable.
 * @param fv The FilledVariables structure.
 * @param index The index to start searching from.
 * @return The index of the next filled variable, or SIZE_MAX if all are
 * unfilled.
 */
extern size_t filled_variables_next_filled(const FilledVariables* fv,
	size_t index
);

/**
 * Create a new FilledVariables structure.
 * @param num_variables The number of variables to track.
 * @return A pointer to the new FilledVariables structure, or NULL on failure.
 */
extern FilledVariables* filled_variables_create(size_t num_variables);

/**
 * Free the memory allocated for a FilledVariables structure.
 * @param fv The FilledVariables structure to free.
 */
extern void filled_variables_destroy(FilledVariables* fv);

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
	size_t* stack_top, const size_t* stop_point, Domain** domains
);

/**
 * Add a change to the change stack.
 * @param stack The DomainChange structure.
 * @param stack_top Pointer to the top of the stack.
 * @param domain_index The index of the domain that changed.
 * @param value The value that was removed from the domain.
 */
extern void domain_change_stack_add(DomainChange* stack, size_t* stack_top,
	size_t domain_index, size_t value
);
