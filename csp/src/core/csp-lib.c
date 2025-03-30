/**
 * @file csp-lib.h
 * Defines functions to load and unload CSP solver library.
 *
 * @author Ch. Demko
 * @date 2024
 */

#include "csp-lib.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// PRIVATE
static int counter = 0;

// Assertions
static void verify(void){
 	assert(!csp_initialised());
}

// PUBLIC
// Initializers / Finishers
bool csp_init(void){
	static bool first = true;

	if(first){
		assert(atexit(verify) == 0);
		first = false;
	}

	if(!counter++){
		assert(printf("CSP initialised\n"));
	}

	return true;
}

bool csp_finish(void){
	if(counter){
		if(!--counter){
			assert(printf("CSP finished\n"));
		}

		return true;
	}else{
		return false;
	}
}

// Getters
bool csp_initialised(void){
  	return counter > 0;
}