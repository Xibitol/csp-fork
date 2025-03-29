cmake_minimum_required(VERSION 3.5)

# Set the project name
project(CSP)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Support for test names
if(POLICY CMP0110)
  cmake_policy(SET CMP0110 NEW)
endif()

# Add the executable
add_executable(solve-queens solve-queens.c)

# Add the library
add_library(csp SHARED csp.c csp.h csp.inc)

# Link the library to the executable
target_link_libraries(solve-queens csp)

# Find test files
file(GLOB FILES "${CMAKE_CURRENT_SOURCE_DIR}/test-*.c")

enable_testing()

find_program(VALGRIND valgrind)

# Add tests
foreach(FILENAME ${FILES})
  get_filename_component(SRC ${FILENAME} NAME)
  get_filename_component(TEST ${FILENAME} NAME_WE)
  add_executable(${TEST} ${SRC} csp.h csp.inc)
  add_dependencies(${TEST} csp)
  target_link_libraries(${TEST} csp)
#  target_link_libraries(${TEST} efence)
  # Add the test
  add_test(${TEST} ./${TEST})
  if(VALGRIND)
    # Add the valgrind test
    add_test(
      "${TEST}[valgrind]" ${VALGRIND} --leak-check=full --quiet --error-exitcode=1 ./${TEST}
    )
  endif()
endforeach()

# Add CPack
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "CSP")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
include(CPack)
