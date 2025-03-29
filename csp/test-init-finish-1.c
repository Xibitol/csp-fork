#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./csp.h"

int main(void) {
  assert(csp_initialised() == false);
  assert(csp_init());
  assert(csp_initialised());
  assert(csp_finish());
  assert(csp_initialised() == false);

  return EXIT_SUCCESS;
}
