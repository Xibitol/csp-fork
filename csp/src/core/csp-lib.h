/**
 * @file csp-lib.h
 * Defines functions to load and unload CSP solver library.
 *
 * @author Ch. Demko
 * @date 2024
 */

#pragma once

#if !defined (_CSP_H_INSIDE) && !defined (CSP_COMPILATION)
#error "Only <csp/csp.h> can be included directly."
#endif

#include <stdbool.h>

// INITIALIZERS / FINISHERS
/**
 * @brief Initialise the CSP library.
 * @return true if the library is initialised, false otherwise.
 * @post The library is initialised.
 */
extern bool csp_init(void);

/**
 * @brief Finish the CSP library.
 * @return true if the library is finished, false otherwise.
 * @post The library is finished.
 */
extern bool csp_finish(void);

// GETTERS
/**
 * @brief Verify if the CSP library is initialised.
 * @return true if the library is initialised, false otherwise.
 * @post The library is not modified.
 */
extern bool csp_initialised(void);