add_library(csp SHARED csp.h
        ./core/csp-lib.c ./core/csp-lib.h
        ./core/csp-constraint.c ./core/csp-constraint.h ./core/csp-constraint.inc.h
        ./core/csp-problem.c ./core/csp-problem.h ./core/csp-problem.inc.h
        ./solver/csp-solver.c ./solver/csp-solver.h)

find_package(Coverage)
enable_coverage()

find_package(HtmlCoverage)
enable_html_coverage()

add_subdirectory(executables)
add_subdirectory(benchmarking)