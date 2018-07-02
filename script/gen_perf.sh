#!/bin/bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)
PROJECT_ROOT_DIR="${SCRIPT_DIR}/.."
BIN_DIR="${PROJECT_ROOT_DIR}/bin"
PERF_RES_DIR="$PROJECT_ROOT_DIR/perf_res"
VIEW_PERF_SCRIPT="$SCRIPT_DIR/view_perf.sh"

TEST_BIN="$BIN_DIR/tests"
USE_CASES=("TraditionalAA.FilterAsset")

cd "${BUILD_DIR}"
cmake .. -DCMAKE_MAKE_PROGRAM=ninja -G Ninja
ninja tests
BUILD_TESTS_RESULT="$?"
TESTS_RESULT=0

if [ "$BUILD_TESTS_RESULT" -ne 0 ]; then
	TESTS_RESULT="$BUILD_TESTS_RESULT"
fi

# Gen use case perf files.
if [ "$TESTS_RESULT" -eq 0 ]; then

	printf "Generating perf profile for Use Case Tests\n"

	mkdir --parents "$PERF_RES_DIR"

	for USE_CASE in ${USE_CASES[@]}; do
		USE_CASE_PERF_RES_DIR="$PERF_RES_DIR/$USE_CASE"
		mkdir --parents "$USE_CASE_PERF_RES_DIR"
		cd "$USE_CASE_PERF_RES_DIR"

		perf record --call-graph fp --all-cpus -- $TEST_BIN --gtest_filter="$USE_CASE"
	done

fi
