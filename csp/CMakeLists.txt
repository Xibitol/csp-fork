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

if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
	target_link_libraries(lib ${EFENCE_LIBRARY})
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

file(GLOB_RECURSE BTEST_FILES CONFIGURE_DEPENDS "${BTEST_DIR}/*.c")
add_executable(btestLib ${BTEST_FILES})
target_link_libraries(btestLib PRIVATE lib)
set_target_properties(btestLib PROPERTIES
	OUTPUT_NAME ${BTEST_EXECUTABLE_NAME}
	RUNTIME_OUTPUT_DIRECTORY ${BTEST_DIR}
)

file(GLOB_RECURSE BTEST_PY_FILES CONFIGURE_DEPENDS "${BTEST_DIR}/python_scripts/*.py")

get_target_property(EXECUTABLE_OUTPUT_PATH btestLib RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(btest
	COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_OUTPUT_PATH}/${BTEST_EXECUTABLE_NAME}"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp/${SUBPROJECT_NAME}
	COMMENT "Running benchmarking test"
)

## Ensure Python scripts have execute permissions
#foreach(PY_FILE ${BTEST_PY_FILES})
#	add_custom_command(TARGET btest PRE_BUILD
#			COMMAND ${CMAKE_COMMAND} -E echo "Setting execute permissions for ${PY_FILE}"
#			COMMAND ${CMAKE_COMMAND} -E chmod +x ${PY_FILE}
#			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp/${SUBPROJECT_NAME}
#	)
#endforeach()

foreach(PY_FILE ${BTEST_PY_FILES})
	MESSAGE(WARNING "Don't forget to set the execute permissions for ${PY_FILE}, chmod +x ${PY_FILE}")
	add_custom_command(TARGET btest POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E echo "Running ${PY_FILE}"
			COMMAND ${Python3_EXECUTABLE} ${PY_FILE}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp/${SUBPROJECT_NAME}
	)
endforeach()