set(SUBPROJECT_NAME csp)
message(STATUS "Configuring ${SUBPROJECT_NAME} sub-project...")

# INPUTS
include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_DIR})

# OUTPUTS
file(GLOB_RECURSE LIBRARY_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.c")
add_library(lib SHARED ${LIBRARY_FILES})
set_target_properties(lib PROPERTIES
	COMPILE_FLAGS "-D CSP_COMPILATION"
	OUTPUT_NAME csp
	LIBRARY_OUTPUT_DIRECTORY ${SOURCE_DIR}/prod
)

find_package(Coverage)
message(STATUS "COVERAGE_EXECUTABLE=${COVERAGE_EXECUTABLE}")
enable_coverage()

find_package(HtmlCoverage)
message(STATUS "HTML_COVERAGE_EXECUTABLE=${HTML_COVERAGE_EXECUTABLE}")
enable_html_coverage()

if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
	target_link_libraries(lib ${EFENCE_LIBRARY})
else()
	get_target_property(LIB_COMPILE_FLAGS lib COMPILE_FLAGS)
	set_target_properties(lib PROPERTIES
		COMPILE_FLAGS "${LIB_COMPILE_FLAGS} -D NDEBUG"
	)
endif()

# TESTS
set(TEST_EXECUTABLE_NAME testCSP)

file(GLOB ABSOLUTE_TEST_DIR "${TEST_DIR}")
file(GLOB_RECURSE TEST_FILES
	RELATIVE "${ABSOLUTE_TEST_DIR}/.."
	CONFIGURE_DEPENDS
	"${TEST_DIR}/*.c"
)
create_test_sourcelist(tests "${TEST_EXECUTABLE_NAME}.c" ${TEST_FILES})

add_executable(testLib ${tests})
add_dependencies(testLib lib)
target_link_libraries(testLib PRIVATE lib)
set_target_properties(testLib PROPERTIES
	OUTPUT_NAME ${TEST_EXECUTABLE_NAME}
	RUNTIME_OUTPUT_DIRECTORY ${TEST_DIR}
)

get_target_property(EXECUTABLE_OUTPUT_PATH testLib RUNTIME_OUTPUT_DIRECTORY)
foreach(FILENAME ${TEST_FILES})
	string(REPLACE ".c" "" NAME ${FILENAME})
	string(REPLACE "${TEST_DIR}/" "" SHORT_NAME ${NAME})

	add_valgrind_test(${SUBPROJECT_NAME} ${SHORT_NAME}
		"${EXECUTABLE_OUTPUT_PATH}/${TEST_EXECUTABLE_NAME}" ${NAME}
	)
endforeach()

# BENCHMARK TEST
set(BTEST_EXECUTABLE_NAME btestCSP)
set(BTEST_PYTHON_MOD_NAME cspbtest_plot)

file(GLOB_RECURSE BTEST_FILES CONFIGURE_DEPENDS "${BTEST_DIR}/*.c")

add_executable(btestLib ${BTEST_FILES})
target_link_libraries(btestLib PRIVATE lib)
set_target_properties(btestLib PROPERTIES
	COMPILE_FLAGS "-D NDEBUG"
	OUTPUT_NAME ${BTEST_EXECUTABLE_NAME}
	RUNTIME_OUTPUT_DIRECTORY ${BTEST_DIR}
)

get_target_property(EXECUTABLE_OUTPUT_PATH btestLib RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(btest
	COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_OUTPUT_PATH}/${BTEST_EXECUTABLE_NAME}"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp/${SUBPROJECT_NAME}
	COMMENT "Running benchmarking test"
)
add_custom_command(TARGET btest POST_BUILD
	COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/${BTEST_DIR}/${BTEST_PYTHON_MOD_NAME}"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp/${SUBPROJECT_NAME}
	COMMENT "Plotting results..."
)