#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./csp.h"

int main(void) {
  assert(csp_init());
  assert(csp_init());
  assert(csp_init());
  assert(csp_finish());
  assert(csp_finish());
  assert(csp_finish());

  return EXIT_SUCCESS;
}
