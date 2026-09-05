/**
 * @file csp-solver-LCV.h
 * Library CSP value heuristics
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 * @copyright GNU Lesser General Public License v3.0
 */

#include <stddef.h>

#include "core/csp-problem.h"
#include "solver/csp-solver.h"
#include "solver/filled-variables.h"
#include "solver/domains.h"

extern int get_constraint_count(const CSPProblem *csp, size_t *values,
															 const void *data, size_t index,
															 FilledVariables *fv,
															 CSPValueChecklist *checklist, Domain **domains);
