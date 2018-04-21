#!/bin/bash

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)
PROJECT_ROOT_DIR="$( cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${PROJECT_ROOT_DIR}/build"
BIN_DIR="${PROJECT_ROOT_DIR}/bin"

RM_FLAGS="-fr"
rm "$RM_FLAGS" "$BUILD_DIR/"*
rm "$RM_FLAGS" "$BIN_DIR/"*
