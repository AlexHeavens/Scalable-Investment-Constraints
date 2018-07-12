#!/bin/bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)
PROJECT_ROOT_DIR="${SCRIPT_DIR}/.."
BUILD_DIR="${PROJECT_ROOT_DIR}/build_debug"

mkdir --parents ${BUILD_DIR}
cd "${BUILD_DIR}"
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja .. 
ninja
