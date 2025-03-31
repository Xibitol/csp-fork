include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(solve-queens solve-queens.c ${CMAKE_SOURCE_DIR}/src/csp.h)
add_dependencies(solve-queens csp)
target_link_libraries(solve-queens csp)

add_executable(solve-sudoku solve-sudoku.c ${CMAKE_SOURCE_DIR}/src/csp.h)
add_dependencies(solve-queens csp)
target_link_libraries(solve-sudoku csp)

add_executable(load-new-sudoku load-new-sudoku.c)