#!/bin/bash
PROJECT_ROOT_DIR="$(git rev-parse --show-toplevel)"
PROJECT_BUILD_DIR="$PROJECT_ROOT_DIR/build"
PROJECT_BIN_DIR="${PROJECT_ROOT_DIR}/bin"

mkdir -p $PROJECT_BUILD_DIR
cd $PROJECT_BUILD_DIR

if [ "$1" == "debug" ]; then
	echo "Generating test in debug mode"
	CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Debug"
fi

# Build.
printf "\n\n"
printf "CMake Build:\n"
printf -- "------------\n"
cmake "$PROJECT_ROOT_DIR" $CMAKE_ARGS -DCMAKE_MAKE_PROGRAM=ninja -G Ninja
ninja tests
MAKE_ALL_TESTS_RESULT="$?"
ALL_TESTS_RESULT=0

if [ "$MAKE_ALL_TESTS_RESULT" -ne 0 ]; then
	ALL_TESTS_RESULT="$MAKE_ALL_TESTS_RESULT"
fi

# Unit and use case tests.
if [ "$ALL_TESTS_RESULT" -eq 0 ]; then
	printf "\n\n"
	printf "Unit / Use-case Tests:\n"
	printf -- "----------------------\n"

	"${PROJECT_BIN_DIR}/tests"
	ALL_TESTS_RESULT="$?"
fi

# Result
printf "\n\nResult: "
if [ "$ALL_TESTS_RESULT" -eq 0 ]; then
    printf "Passed.\n\n"
else
	>&2 printf "Test failure.\n\n"
fi

exit "$ALL_TESTS_RESULT"
