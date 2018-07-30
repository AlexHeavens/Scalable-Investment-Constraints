#!/bin/bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)
PROJECT_ROOT_DIR="${SCRIPT_DIR}/.."
BUILD_DIR="${PROJECT_ROOT_DIR}/build_debug"
BIN_DIR="${PROJECT_ROOT_DIR}/bin_debug"

mkdir --parents ${BUILD_DIR} ${BIN_DIR}
cd "${BUILD_DIR}"
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_MAKE_PROGRAM=ninja -G Ninja ..
ninja

BUILD_RESULT="$?"
exit $BUILD_RESULT
