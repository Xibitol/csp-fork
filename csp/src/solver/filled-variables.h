/**
 * @file filled-variables.h
 * Functions to manipulate bitset structure
 * to track variables that have been filled during solving
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#if !defined(_CSP_H_INSIDE) && !defined(CSP_COMPILATION)
#error "Only <csp/csp.h> can be included directly."
#endif

#include <stdbool.h>
#include <stddef.h>

/**
 * The FilledVariables structure
 */
typedef struct _FilledVariables FilledVariables;

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
																						 size_t index);

/**
 * Get the next filled variable.
 * @param fv The FilledVariables structure.
 * @param index The index to start searching from.
 * @return The index of the next filled variable, or SIZE_MAX if all are
 * unfilled.
 */
extern size_t filled_variables_next_filled(const FilledVariables* fv,
																					 size_t index);

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
 * Get size of the filled variables bitset
 * @param fv The FilledVaraibles structure
 */
extern size_t filled_variables_get_size(FilledVariables* fv);
