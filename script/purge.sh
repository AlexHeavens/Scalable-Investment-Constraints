#!/bin/bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)
PROJECT_ROOT_DIR="$( cd "${SCRIPT_DIR}/.." && pwd)"

RM_FLAGS="-fr"
rm "$RM_FLAGS" "${PROJECT_ROOT_DIR}/bin/*" "${PROJECT_ROOT_DIR}/bin_debug/*"
rm "$RM_FLAGS" "${PROJECT_ROOT_DIR}/build/*" "${PROJECT_ROOT_DIR}/build_debug/*"
