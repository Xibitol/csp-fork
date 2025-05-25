find_program(DOXYGEN_EXECUTABLE doxygen)
message(STATUS "DOXYGEN_EXECUTABLE=${DOXYGEN_EXECUTABLE}")
mark_as_advanced(DOXYGEN_EXECUTABLE)

find_package(Sphinx)
message(STATUS "SPHINX_EXECUTABLE=${SPHINX_EXECUTABLE}")
mark_as_advanced(SPHINX_EXECUTABLE)

if(SPHINX_EXECUTABLE AND DOXYGEN_EXECUTABLE)

    add_custom_target(docs
            COMMAND
            ${SPHINX_EXECUTABLE}
            -q
            -b html
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}/html"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Building HTML documentation with Sphinx."
    )

    add_custom_command(TARGET docs POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "HTML documentation is in ${CMAKE_CURRENT_BINARY_DIR}/html"
            COMMENT "HTML documentation is in ${CMAKE_CURRENT_BINARY_DIR}/html"
    )

    install(
            DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
            DESTINATION share/docs
            COMPONENT "Docs"
            OPTIONAL
    )
endif()