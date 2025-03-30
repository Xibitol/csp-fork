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

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

enable_testing()

find_package(CppCheck)
enable_cppcheck()

find_package(CcLint)
enable_cclint()

find_package(ClangFormat)
enable_clang_format()

find_package(FlawFinder)
enable_flaw_finder()

add_subdirectory(src)
add_subdirectory(test)

# Add CPack
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.vscode/;/.venv/;/.git/;/.idea/;~$;${CPACK_SOURCE_IGNORE_FILES}")
set(CPACK_PACKAGE_NAME "CSP")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
include(CPack)
