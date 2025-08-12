/**
 * @file filled-variables.c
 * Functions to manipulate the bitset structure
 * to track variables that have been filled during solving
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "filled-variables.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "filled-variables.inc.h"

// Initialize the structure
FilledVariables* filled_variables_create(size_t num_variables) {
	FilledVariables* fv = malloc(sizeof(FilledVariables));
	if (fv != NULL) {
    fv->size = num_variables;
    size_t num_bytes = (num_variables + 7) / 8;	 // Round up to the nearest byte
    fv->bitset = calloc(num_bytes, sizeof(uint8_t));
    if (fv->bitset == NULL) {
      free(fv);
      return NULL;
    }
	}
	return fv;
}

void filled_variables_mark_filled(FilledVariables* fv, size_t index) {
	fv->bitset[index / 8] |= (1 << (index % 8));
}

void filled_variables_mark_unfilled(FilledVariables* fv, size_t index) {
	fv->bitset[index / 8] &= ~(1 << (index % 8));
}

// Check if a variable is filled
bool filled_variables_is_filled(const FilledVariables* fv, size_t index) {
	return fv->bitset[index / 8] & (1 << (index % 8));
}

bool filled_variables_all_filled(const FilledVariables* fv) {
	size_t full_bytes = fv->size / 8;			 // Number of fully used bytes
	size_t remaining_bits = fv->size % 8;	 // Remaining bits in the last byte

	// Check all full bytes
	for (size_t i = 0; i < full_bytes; i++) {
		if (fv->bitset[i] != 0xFF) {
			return false;
		}
	}

	// Check the last byte if there are remaining bits
	if (remaining_bits > 0) {
		uint8_t mask = (1 << remaining_bits) - 1;	 // Mask for the valid bits
		if ((fv->bitset[full_bytes] & mask) != mask) {
			return false;
		}
	}

	return true;
}

size_t filled_variables_next_unfilled(const FilledVariables* fv,
																			size_t start_index) {
	if (start_index >= fv->size) {
		return SIZE_MAX;	// Invalid index
	}
	size_t start_byte = start_index / 8;
	size_t start_bit = start_index % 8;

	// Check the first byte from the start bit
	if (start_bit > 0) {
		// Mask for bits from start_bit onward
		uint8_t mask = ~(0xFF << start_bit);
		if ((fv->bitset[start_byte] & mask) != mask) {
			for (size_t bit = start_bit; bit < 8; bit++) {
				if (!(fv->bitset[start_byte] & (1 << bit))) {
					return start_byte * 8 + bit;
				}
			}
		}
		start_byte++;
		if (start_byte * 8 >= fv->size) {
			return SIZE_MAX;	// Invalid index
		}
	}

	// Check all full bytes
	for (size_t i = start_byte; i < fv->size / 8; i++) {
		if (fv->bitset[i] != 0xFF) {
			for (size_t bit = 0; bit < 8; bit++) {
				if (!(fv->bitset[i] & (1 << bit))) {
					return i * 8 + bit;
				}
			}
		}
	}

	// Check the last byte if there are remaining bits
	size_t remaining_bits = fv->size % 8;
	if (remaining_bits > 0) {
		// Mask for the valid bits
		uint8_t mask = (1 << remaining_bits) - 1;
		if ((fv->bitset[fv->size / 8] & mask) != mask) {
			for (size_t bit = 0; bit < remaining_bits; bit++) {
				if (!(fv->bitset[fv->size / 8] & (1 << bit))) {
					return (fv->size / 8) * 8 + bit;
				}
			}
		}
	}

	return SIZE_MAX;
}

size_t filled_variables_next_filled(const FilledVariables* fv,
																		size_t start_index) {
	if (start_index >= fv->size) {
		return SIZE_MAX;	// Invalid index
	}
	size_t start_byte = start_index / 8;
	size_t start_bit = start_index % 8;

	// Check the first byte from the start bit
	if (start_bit > 0) {
		uint8_t mask = 0xFF << start_bit;	 // Mask for bits from start_bit onward
		if (fv->bitset[start_byte] & mask) {
			for (size_t bit = start_bit; bit < 8; bit++) {
				if (fv->bitset[start_byte] & (1 << bit)) {
					return start_byte * 8 + bit;
				}
			}
		}
		start_byte++;
		if (start_byte * 8 >= fv->size) {
			return SIZE_MAX;	// Invalid index
		}
	}

	// Check all full bytes
	for (size_t i = start_byte; i < fv->size / 8; i++) {
		if (fv->bitset[i] != 0x00) {
			for (size_t bit = 0; bit < 8; bit++) {
				if (fv->bitset[i] & (1 << bit)) {
					return i * 8 + bit;
				}
			}
		}
	}

	// Check the last byte if there are remaining bits
	size_t remaining_bits = fv->size % 8;
	if (remaining_bits > 0) {
		uint8_t mask = (1 << remaining_bits) - 1;	 // Mask for the valid bits
		if (fv->bitset[fv->size / 8] & mask) {
			for (size_t bit = 0; bit < remaining_bits; bit++) {
				if (fv->bitset[fv->size / 8] & (1 << bit)) {
					return (fv->size / 8) * 8 + bit;
				}
			}
		}
	}

	return SIZE_MAX;
}

// Free the structure
void filled_variables_destroy(FilledVariables* fv) {
	free(fv->bitset);
	free(fv);
}

size_t filled_variables_get_size(FilledVariables* fv) { return fv->size; }
