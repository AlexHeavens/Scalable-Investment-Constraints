#!/bin/bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)
PROJECT_ROOT_DIR="${SCRIPT_DIR}/.."
BUILD_DIR="${PROJECT_ROOT_DIR}/build"

cd "${BUILD_DIR}"
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
