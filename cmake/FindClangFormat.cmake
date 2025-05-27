find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
mark_as_advanced(CLANG_FORMAT_EXECUTABLE)

function(enable_clang_format)
    if(CLANG_FORMAT_EXECUTABLE)
        add_custom_target(clang-format
			COMMAND
			${CLANG_FORMAT_EXECUTABLE}
			-i
			-style=file
			"${CMAKE_SOURCE_DIR}/csp/src/*.h"
			"${CMAKE_SOURCE_DIR}/csp/src/core/*.c"
			"${CMAKE_SOURCE_DIR}/csp/src/core/*.h"
			"${CMAKE_SOURCE_DIR}/csp/src/solver/*.c"
			"${CMAKE_SOURCE_DIR}/csp/src/solver/*.h"
			# "${CMAKE_SOURCE_DIR}/csp/src/util/*.c"
			"${CMAKE_SOURCE_DIR}/csp/src/util/*.h"
			"${CMAKE_SOURCE_DIR}/csp/test/core/*.c"
			"${CMAKE_SOURCE_DIR}/csp/test/solver/*.c"
			"${CMAKE_SOURCE_DIR}/csp/btest/*.c"
			"${CMAKE_SOURCE_DIR}/csp/btest/*.h"
			COMMENT "Format sources."
        )
    endif(CLANG_FORMAT_EXECUTABLE)
endfunction(enable_clang_format)