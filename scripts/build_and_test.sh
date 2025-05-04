#!/bin/bash
set -e
set -x

# Get project root directory
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"

# Check for --no-clean flag
if [[ "$1" != "--no-clean" ]]; then
    rm -rf "${PROJECT_ROOT}/build"
fi

cmake -S "${PROJECT_ROOT}" -B "${PROJECT_ROOT}/build" -DCMAKE_BUILD_TYPE=Debug
cmake --build "${PROJECT_ROOT}/build"

CTEST_OUTPUT_ON_FAILURE=1 ctest --test-dir "${PROJECT_ROOT}/build"