//
// Created by adrien on 13/05/25.
//

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

// Mark a variable as filled
void filled_variables_mark_filled(FilledVariables* fv, size_t index) {
	fv->bitset[index / 8] |= (1 << (index % 8));
}

// Check if a variable is filled
bool filled_variables_is_filled(const FilledVariables* fv, size_t index) {
	return fv->bitset[index / 8] & (1 << (index % 8));
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

void domain_print(const Domain* domain) {
	for (size_t i = 0; i < domain->amount; i++) {
		printf("%zu ", domain->values[i]);
	}
	printf("\n");
}

DomainChange* domain_change_stack_create(size_t size) {
	DomainChange* stack = malloc(size * sizeof(DomainChange));
	if (stack == NULL) {
		perror("malloc");
		return NULL;
	}
	return stack;
}

void domain_change_stack_destroy(DomainChange* stack) { free(stack); }

void domain_change_stack_restore(DomainChange* stack, size_t* stack_top,
																 size_t* stop_point, Domain** domains) {
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