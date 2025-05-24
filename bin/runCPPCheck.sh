#!/bin/bash

# This script runs cppcheck on the specified source directory or files.
if [[ $# -lt 3 ]]; then
    echo "Usage: $0 /path/to/cppcheck /path/to/file.c /path/to/suppressions"
    exit 1
fi

CPPCHECK_EXEC="$1"
SOURCE_FILE="$2"
SUPPRESSIONS_FILE="$3"
shift

# Example usage: run cppcheck on the current directory
"$CPPCHECK_EXEC" -DCSP_COMPILATION -D_CSP_H_INSIDE --language=c --std=c11 --enable=all --inconclusive --suppressions-list="$SUPPRESSIONS_FILE" --error-exitcode=0 "$SOURCE_FILE" || exit 1