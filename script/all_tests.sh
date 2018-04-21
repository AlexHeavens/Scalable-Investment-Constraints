#!/usr/bin/env bash
PROJECT_ROOT_DIR="$(git rev-parse --show-toplevel)"
PROJECT_BUILD_DIR="$PROJECT_ROOT_DIR/build"
PROJECT_BIN_DIR="${PROJECT_ROOT_DIR}/bin"

mkdir -p $PROJECT_BUILD_DIR
cd $PROJECT_BUILD_DIR

if [ "$1" == "debug" ]; then
	echo "Generating test in debug mode"
	CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Debug"
	cmake $PROJECT_ROOT_DIR clean
	make clean
fi

cmake "$PROJECT_ROOT_DIR" $CMAKE_ARGS
make all_tests
MAKE_ALL_TESTS_RESULT="$?"

if [ "$MAKE_ALL_TESTS_RESULT" -ne 0 ]; then
	ALL_TESTS_RESULT="$MAKE_ALL_TESTS_RESULT"
else
	"${PROJECT_BIN_DIR}/all_tests"
	ALL_TESTS_RESULT="$?"
fi

if [ "$ALL_TESTS_RESULT" -eq 0 ]; then
    printf "Passed.\n"
else
	>&2 printf "Test failure.\n"
fi

exit "$ALL_TESTS_RESULT"
