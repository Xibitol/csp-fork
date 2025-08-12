/**
 * @file filled_variables.inc.h
 * Bitset structure to track variables that have been filled during solving
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "filled-variables.h"

/**
 * Structure to track filled variables in a CSP problem.
 * It uses a bitset to efficiently track which variables are filled.
 */
struct _FilledVariables {
	size_t size;			// Number of variables
	uint8_t* bitset;	// Bitset to track filled variables
};
