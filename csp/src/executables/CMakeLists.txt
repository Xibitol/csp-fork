include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(solve-queens solve-queens.c ${CMAKE_SOURCE_DIR}/src/csp.h)
add_dependencies(solve-queens csp)
target_link_libraries(solve-queens csp)

add_executable(solve-sudoku-v1 solve-sudoku-v1.c ../solver/sudoku-basic-solve.c ../solver/sudoku-basic-solve.h)
target_link_libraries(solve-sudoku-v1 csp)

add_executable(solve-sudoku-v2 solve-sudoku-v2.c ../solver/sudoku-basic-solve-2.c ../solver/sudoku-basic-solve-2.h)
target_link_libraries(solve-sudoku-v2 csp)

add_executable(load-new-sudoku load-new-sudoku.c)