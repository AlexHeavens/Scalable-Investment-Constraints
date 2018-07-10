#!/bin/bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)
PROJECT_ROOT_DIR="${SCRIPT_DIR}/.."
BIN_DIR="${PROJECT_ROOT_DIR}/bin"
BUILD_DIR="${PROJECT_ROOT_DIR}/build"
PERF_RES_DIR="$PROJECT_ROOT_DIR/perf_res"
VIEW_PERF_SCRIPT="$SCRIPT_DIR/view_perf.sh"

USE_CASE_BIN_DIR="$BIN_DIR/use_case"

rm "$PERF_RES_DIR"/* -r
cd "${BUILD_DIR}"
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebugInfo -DCMAKE_MAKE_PROGRAM=ninja -G Ninja
ninja
BUILD_RESULT="$?"
RESULT=0

if [ "$BUILD_RESULT" -ne 0 ]; then
	RESULT="$BUILD_RESULT"
fi

# Gen use case perf files.
if [ "$RESULT" -eq 0 ]; then

	printf "Generating perf profile for Use Case Tests\n"

	mkdir --parents "$PERF_RES_DIR"

	for USE_CASE_BIN in "$USE_CASE_BIN_DIR"/*; do
		USE_CASE=$(basename "$USE_CASE_BIN")
		USE_CASE_PERF_RES_DIR="$PERF_RES_DIR/$USE_CASE"

		printf "Use case: $USE_CASE\n"
		mkdir --parents "$USE_CASE_PERF_RES_DIR"
		cd "$USE_CASE_PERF_RES_DIR"

		perf record --call-graph fp --all-cpus -- $USE_CASE_BIN --benchmark_filter="$USE_CASE" --benchmark_min_time=0.1
	done

fi
