/**
 * @file domains.c
 * Functions to manipulate domain related structures
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include "domains.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "domains.inc.h"
#include "filled-variables.h"

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

size_t domain_get_value(Domain* domain, size_t index) {
	return domain->values[index];
}

size_t domain_get_amount(Domain* domain) { return domain->amount; }

void domain_remove_value(Domain* domain, size_t index) {
	domain->amount--;
	for (size_t k = index; k < domain->amount; k++) {
		domain->values[k] = domain->values[k + 1];
	}
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
