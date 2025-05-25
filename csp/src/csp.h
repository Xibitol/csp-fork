/* PIA CSP (Library and benchmarking program)
Copyright &copy; 2025 - G team

These programs is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

These last are distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU General Public License and the GNU
Lesser General Public License along with the programs (COPYING &
COPYING.LESSER). If not, see https://www.gnu.org/licenses/.
*/

/**
 * @file csp.h
 * Aggregation header of the CSP solver library.
 *
 * @author Xibitol <xibitol@pimous.dev>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#define _CSP_H_INSIDE

#include "core/csp-lib.h"
#include "core/csp-constraint.h"
#include "core/csp-problem.h"

#include "solver/csp-solver.h"
#include "solver/csp-solver-fc.h"
#include "solver/csp-solver-ovars.h"
// #include "solver/csp-solver-ovals.h"

#include "solver/types-and-structs.h"

#undef _CSP_H_INSIDE