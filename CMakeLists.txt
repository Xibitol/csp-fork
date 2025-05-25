cmake_minimum_required(VERSION 3.5)
enable_testing()

project(CSP
	DESCRIPTION "A CSP solver forked to increase performances."
	VERSION 1.0.0
	LANGUAGES C
	HOMEPAGE_URL "https://github.com/Xibitol/csp-fork" 
)

set(SOURCE_DIR src)
set(TEST_DIR test)
set(BTEST_DIR btest)

# OPTIONS
add_compile_options(-Wall -Wextra -Wpedantic)

if(POLICY CMP0110)
	cmake_policy(SET CMP0110 NEW)
endif()
set(CMAKE_CTEST_ARGUMENTS --output-on-failure)
set(VALGRIND_ARGUMENTS
	-s --leak-check=full --show-leak-kinds=all --error-exitcode=1
	--track-origins=yes
)

# EXTERNAL LIBRARIES
message(STATUS "Detecting external libraries...")

find_library(EFENCE_LIBRARY efence)
message(STATUS "EFENCE_LIBRARY=${EFENCE_LIBRARY}")

# EXTERNAL PACKAGES/PROGRAMS
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

message(STATUS "Detecting external programs...")

find_package(Valgrind)
message(STATUS "VALGRIND_EXECUTABLE=${VALGRIND_EXECUTABLE}")

find_package(Python3 COMPONENTS Interpreter)

find_package(CppCheck)
message(STATUS "CPPCHECK_EXECUTABLE=${CPPCHECK_EXECUTABLE}")
enable_cppcheck()

find_package(CcLint)
message(STATUS "CCLINT_EXECUTABLE=${CCLINT_EXECUTABLE}")
enable_cclint()

find_package(FlawFinder)
message(STATUS "FLAWFINDER_EXECUTABLE=${FLAW_FINDER_EXECUTABLE}")
enable_flaw_finder()

find_package(ClangFormat)
message(STATUS "CLANG_FORMAT_EXECUTABLE=${CLANG_FORMAT_EXECUTABLE}")
enable_clang_format()

# MACROS
macro(add_valgrind_test SUBPROJECT_NAME NAME EXECUTABLE)
	set(EXECUTABLE_PATH
		${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}
	)

	set(WORKING_TEST_DIR ${CMAKE_SOURCE_DIR}/tmp/${SUBPROJECT_NAME})
	make_directory(${WORKING_TEST_DIR})

	add_test(
		NAME "(${SUBPROJECT_NAME}) ${NAME}"
		COMMAND ${EXECUTABLE_PATH} ${ARGN}
		WORKING_DIRECTORY ${WORKING_TEST_DIR}
	)
	if(VALGRIND_EXECUTABLE AND USE_VALGRIND)
		add_test(
			NAME "(${SUBPROJECT_NAME})[mem-check] ${NAME}"
			COMMAND ${VALGRIND_EXECUTABLE}
			${VALGRIND_ARGUMENTS} ${EXECUTABLE_PATH} ${ARGN}
			WORKING_DIRECTORY ${WORKING_TEST_DIR}
		)
	endif()
endmacro()

# PROJECTS
add_subdirectory(csp)
add_subdirectory(docs)