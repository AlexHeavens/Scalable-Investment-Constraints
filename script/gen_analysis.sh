#!/bin/bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)
PROJECT_ROOT_DIR="${SCRIPT_DIR}/.."
BIN_DIR="${PROJECT_ROOT_DIR}/bin"
RESULTS_PATH="$PROJECT_ROOT_DIR/perf_res/benchmark_results.json"

"$BIN_DIR/benchmarks" --benchmark_format=json --benchmark_out="$RESULTS_PATH" > /dev/null

PLOT_DIR="$PROJECT_ROOT_DIR/analysis/plots"
mkdir --parents "$PLOT_DIR"
rm -r "$PLOT_DIR/*"

GEN_PLOTS_SCRIPT="$SCRIPT_DIR/generate_plots.py"
python3 "$GEN_PLOTS_SCRIPT"
