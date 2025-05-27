find_program(COVERAGE_EXECUTABLE lcov)
mark_as_advanced(COVERAGE_EXECUTABLE)
include(CMakeDependentOption)
cmake_dependent_option(BUILD_COVERAGE "Enabling coverage support" OFF "COVERAGE_EXECUTABLE" OFF)
mark_as_advanced(BUILD_COVERAGE)

if(BUILD_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif()

function(enable_coverage)
    if(BUILD_COVERAGE)
        add_custom_target(coverage
			${COVERAGE_EXECUTABLE}
			--base-directory ${CMAKE_SOURCE_DIR}/csp/src
			--directory ${CMAKE_CURRENT_BINARY_DIR}
			--capture
			--output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage.info
			--quiet
			COMMENT "Building coverage info"
			VERBATIM
        )
        add_custom_command(TARGET coverage POST_BUILD
			COMMAND
			${COVERAGE_EXECUTABLE}
			--remove ${CMAKE_CURRENT_BINARY_DIR}/coverage.info "${CMAKE_SOURCE_DIR}/csp/test/*" "${CMAKE_CURRENT_BINARY_DIR}/testCSP.c" "${CMAKE_SOURCE_DIR}/usr/*" --output-file ${CMAKE_CURRENT_BINARY_DIR}/coverage.info.cleaned
			--quiet
			--ignore-errors unused
			COMMAND
			${COVERAGE_EXECUTABLE}
			--add-tracefile ${CMAKE_CURRENT_BINARY_DIR}/coverage.info.cleaned
			--output-file ${CMAKE_BINARY_DIR}/coverage.info
			--quiet
        )
        set_directory_properties(PROPERTIES
			ADDITIONAL_CLEAN_FILES "${CMAKE_BINARY_DIR}/coverage.info;coverage.info"
        )

    endif()
endfunction(enable_coverage)