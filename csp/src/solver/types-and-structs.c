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
#include "solver/filled-variables.h"


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

void print_domains(const Domain** domains, const size_t num_domains) {
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
