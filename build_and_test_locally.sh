#!/usr/bin/env sh
set -e

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS="--coverage"
make
ctest --output-on-failure
gcovr --gcov-ignore-parse-errors=suspicious_hits.warn_once_per_file \
    -r .. \
    --filter '.*/src/.*\.c$' \
    --html --html-details -o coverage.html