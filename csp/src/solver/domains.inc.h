/**
 * @file domains.inc.h
 * File containing the domain-related structures
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <stddef.h>

#include "domains.h"

/**
 * Structure to represent the domain of a variable in a CSP problem.
 * It contains the number of values in the domain and an array of values.
 */
struct _Domain {
	size_t amount;
	size_t values[];
};

/**
 * Structure to track changes in the domain of a variable during forward
 * checking. Use as a stack to store the changes.
 * It stores the index of the domain and the value that was removed.
 */
struct _DomainChange {
	size_t domain_index;
	size_t value;
};
