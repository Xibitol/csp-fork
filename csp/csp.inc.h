/**
 * @brief The constraint of a CSP problem.
 * @var check The check function of the constraint.
 * @var arity The arity of the constraint.
 * @var variables The variables of the constraint.
 */
struct _CSPConstraint {
  CSPChecker *check;
  size_t arity;
  size_t variables[];
};

/**
 * @brief The CSP problem.
 * @var num_domains The number of variables.
 * @var domains The domains of the variables.
 * @var num_constraints The number of constraints.
 * @var constraints The constraints of the problem.
 */
struct _CSPProblem {
  size_t num_domains;
  size_t *domains;
  size_t num_constraints;
  CSPConstraint **constraints;
};
