/**
* @file types-and-structs.c
 * File containing the definition of the types and structures used in the CSP
 * And the functions to manipulate them.
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "solver/types-and-structs.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Initialize the structure
FilledVariables* filled_variables_create(size_t num_variables) {
	FilledVariables* fv = malloc(sizeof(FilledVariables));
	if (fv == NULL) {
		perror("malloc");
		return NULL;
	}
	fv->size = num_variables;
	size_t num_bytes = (num_variables + 7) / 8;	 // Round up to the nearest byte
	fv->bitset = calloc(num_bytes, sizeof(uint8_t));
	if (fv->bitset == NULL) {
		perror("calloc");
		free(fv);
		return NULL;
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
	size_t start_index
){
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
			return SIZE_MAX; // Invalid index
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
	size_t start_index
){
	if (start_index >= fv->size) {
		return SIZE_MAX; // Invalid index
	}
	size_t start_byte = start_index / 8;
	size_t start_bit = start_index % 8;

	// Check the first byte from the start bit
	if (start_bit > 0) {
		uint8_t mask = 0xFF << start_bit; // Mask for bits from start_bit onward
		if (fv->bitset[start_byte] & mask) {
			for (size_t bit = start_bit; bit < 8; bit++) {
				if (fv->bitset[start_byte] & (1 << bit)) {
					return start_byte * 8 + bit;
				}
			}
		}
		start_byte++;
		if (start_byte * 8 >= fv->size) {
			return SIZE_MAX; // Invalid index
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
		uint8_t mask = (1 << remaining_bits) - 1; // Mask for the valid bits
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

Domain* domain_create(size_t size) {
	Domain* domain = malloc(sizeof(Domain) + size * sizeof(size_t));
	if (domain == NULL) {
		perror("malloc");
		return NULL;
	}
	domain->amount = size;
	for (size_t i = 0; i < size; i++) {
		domain->values[i] = i;
	}
	return domain;
}

void domain_destroy(Domain* domain) { free(domain); }

void print_domain(const Domain* domain) {
	for (size_t i = 0; i < domain->amount; i++) {
		printf("%zu ", domain->values[i]);
	}
	printf("\n");
}

void print_domains(const Domain **domains, const size_t num_domains) {
	for (size_t i = 0; i < num_domains; i++) {
		print_domain(domains[i]);
	}
	printf("\n");
}

DomainChange* domain_change_stack_create(const size_t size) {
	DomainChange* stack = malloc(size * sizeof(DomainChange));
	if (stack == NULL) {
		perror("malloc");
		return NULL;
	}
	return stack;
}

void domain_change_stack_destroy(DomainChange* stack) { free(stack); }

void domain_change_stack_restore(const DomainChange* stack, size_t* stack_top,
																 const size_t* stop_point, Domain** domains) {
	while (*stack_top > *stop_point) {
		// Only restore changes until stop point
		(*stack_top)--;
		size_t domain_index = stack[*stack_top].domain_index;
		size_t value = stack[*stack_top].value;

		domains[domain_index]->values[domains[domain_index]->amount] = value;
		domains[domain_index]->amount++;
	}
}

void domain_change_stack_add(DomainChange* stack, size_t* stack_top,
														 size_t domain_index, size_t value) {
	stack[*stack_top].domain_index = domain_index;
	stack[*stack_top].value = value;
	(*stack_top)++;
}