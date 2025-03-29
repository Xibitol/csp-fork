#ifndef CSP_H_
#define CSP_H_

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief The constraint of a CSP problem.
 */
typedef struct _CSPConstraint CSPConstraint;
/**
 * @brief The CSP problem.
 */
typedef struct _CSPProblem CSPProblem;
/**
 * @brief The check function of a CSP constraint.
 * @param constraint The constraint to check.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @return true if the constraint is satisfied, false otherwise.
 * @pre constraint != NULL
 * @pre values != NULL
 */
typedef bool CSPChecker(const CSPConstraint *, const size_t *, const void *);

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
/**
 * @brief Verify if the CSP library is initialised.
 * @return true if the library is initialised, false otherwise.
 * @post The library is not modified.
 */
extern bool csp_initialised(void);

/**
 * @brief Create a constraint with the specified arity and check function.
 * @param arity The arity of the constraint.
 * @param check The check function of the constraint.
 * @return The constraint created or NULL if an error occurred.
 * @pre The csp library is initialised.
 * @pre arity > 0
 * @pre check != NULL
 * @post The constraint variables are initialised to 0.
 * @post The constraint arity is set to the specified arity.
 * @post The constraint check function is set to the specified check function.
 */
extern CSPConstraint *csp_constraint_create(size_t arity, CSPChecker *check);
/**
 * @brief Destroy the constraint.
 * @param constraint The constraint to destroy.
 * @pre The csp library is initialised.
 * @pre constraint != NULL
 * @post The constraint variables are freed.
 * @post The constraint is freed.
 */
extern void csp_constraint_destroy(CSPConstraint *constraint);
/**
 * @brief Get the arity of the constraint.
 * @param constraint The constraint to get the arity.
 * @return The arity of the constraint.
 * @pre The csp library is initialised.
 */
extern size_t csp_constraint_get_arity(const CSPConstraint *constraint);
/**
 * @brief Get the check function of the constraint.
 * @param constraint The constraint to get the check function.
 * @return The check function of the constraint.
 * @pre The csp library is initialised.
 */
extern CSPChecker *csp_constraint_get_check(const CSPConstraint *constraint);
/**
 * @brief Set the variable of the constraint at the specified index.
 * @param constraint The constraint to set the variable.
 * @param index The index of the variable.
 * @param variable The variable to set.
 * @pre The csp library is initialised.
 * @pre index < constraint->arity
 */
extern void csp_constraint_set_variable(CSPConstraint *constraint, size_t index, size_t variable);
/**
 * @brief Get the variable of the constraint at the specified index.
 * @param constraint The constraint to get the variable.
 * @param index The index of the variable.
 * @return The variable at the specified index.
 * @pre The csp library is initialised.
 * @pre index < constraint->arity
 */
extern size_t csp_constraint_get_variable(const CSPConstraint *constraint, size_t index);
/**
 * @brief Verify if the contraint can be checked.
 * @param constraint The constraint to verify.
 * @param index The index of the last variable.
 * @return true if the constraint can be checked, false otherwise.
 * @pre The csp library is initialised.
 * @pre constraint != NULL
 */
extern bool csp_constraint_to_check(const CSPConstraint *constraint, size_t index);

/**
 * @brief Create a CSP problem with the specified number of variables and constraints.
 * @param num_domains The number of variables of the CSP problem.
 * @param num_constraints The number of constraints of the CSP problem.
 * @return The CSP problem created or NULL if an error occurred.
 * @pre The csp library is initialised.
 * @pre num_variables > 0
 * @pre num_constraints > 0
 * @post The CSP problem domains are initialised to 0.
 * @post The CSP problem constraints are initialised to NULL.
 * @post The CSP problem number of domains is set to the specified number of domains.
 * @post The CSP problem number of constraints is set to the specified number of constraints.
 */
extern CSPProblem *csp_problem_create(size_t num_domains, size_t num_constraints);
/**
 * @brief Destroy the CSP problem.
 * @param csp The CSP problem to destroy.
 * @pre The csp library is initialised.
 * @post The CSP problem constraints are freed.
 * @post The CSP problem domains is freed.
 * @post The CSP problem is freed.
 */
extern void csp_problem_destroy(CSPProblem *csp);
/**
 * @brief Get the number of constraints of the CSP problem.
 * @param csp The CSP problem to get the number of constraints.
 * @return The number of constraints of the CSP problem.
 * @pre The csp library is initialised.
 */
extern size_t csp_problem_get_num_constraints(const CSPProblem *csp);
/**
 * @brief Set the constraint of the CSP problem at the specified index.
 * @param csp The CSP problem to set the constraint.
 * @param index The index of the constraint.
 * @param constraint The constraint to set.
 * @pre The csp library is initialised.
 * @pre index < csp->num_constraints
 * @pre constraint != NULL
 */
extern void csp_problem_set_constraint(CSPProblem *csp, size_t index, CSPConstraint *constraint);
/**
 * @brief Get the constraint of the CSP problem at the specified index.
 * @param csp The CSP problem to get the constraint.
 * @param index The index of the constraint.
 * @return The constraint at the specified index.
 * @pre The csp library is initialised.
 */
extern CSPConstraint *csp_problem_get_constraint(const CSPProblem *csp, size_t index);
/**
 * @brief Get the number of domains of the CSP problem.
 * @param csp The CSP problem to get the number of domains.
 * @return The number of domains of the CSP problem.
 * @pre The csp library is initialised.
 */
extern size_t csp_problem_get_num_domains(const CSPProblem *csp);
/**
 * @brief Set the domain of the CSP problem at the specified index.
 * @param csp The CSP problem to set the domain.
 * @param index The index of the domain.
 * @param domain The domain to set.
 * @pre The csp library is initialised.
 * @pre index < csp->num_domains
 */
extern void csp_problem_set_domain(CSPProblem *csp, size_t index, size_t domain);
/**
 * @brief Get the domain of the CSP problem at the specified index.
 * @param csp The CSP problem to get the domain.
 * @param index The index of the domain.
 * @return The domain at the specified index.
 * @pre The csp library is initialised.
 * @pre index < csp->num_domains
 */
extern size_t csp_problem_get_domain(const CSPProblem *csp, size_t index);
/**
 * @brief Verify if the CSP problem is consistent at the specified index.
 * @param csp The CSP problem to verify.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @param index The index of the current variable.
 * @return true if the CSP problem is consistent, false otherwise.
 * @pre The csp library is initialised.
 */
extern bool csp_problem_is_consistent(const CSPProblem *csp, const size_t *values, const void *data, size_t index);
/**
 * @brief Solve the CSP problem using backtracking.
 * @param csp The CSP problem to solve.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @param index The index of the variable to set.
 * @return true if the CSP problem is solved, false otherwise.
 * @pre The csp library is initialised.
 * @post The values are assigned to the solution.
 */
extern bool csp_problem_backtrack(const CSPProblem *csp, size_t *values, const void *data, size_t index);
/**
 * @brief Solve the CSP problem using backtracking.
 * @param csp The CSP problem to solve.
 * @param values The values of the variables.
 * @param data The data to pass to the check function.
 * @return true if the CSP problem is solved, false otherwise.
 * @pre The csp library is initialised.
 * @post The values are assigned to the solution.
 */
extern bool csp_problem_solve(const CSPProblem *csp, size_t *values, const void *data);

#endif  // CSP_H_
