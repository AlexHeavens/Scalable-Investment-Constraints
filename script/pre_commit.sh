#!/usr/bin/env bash
PROJECT_ROOT_DIR="$(git rev-parse --show-toplevel)"
PROJECT_SCRIPTS_DIR="$PROJECT_ROOT_DIR/script"

# Unstaged new or modified files could false-pass commit tests.UNADDED_FILES=$(git ls-files --others --exclude-standard | head -n=-1)
UNADDED_FILES=$(git ls-files --others --exclude-standard)
UNADDED_FILE_COUNT=$(echo -n "$UNADDED_FILES" | wc -l)
if [ "$UNADDED_FILE_COUNT" -ne 0 ]; then
    >&2 printf "Unstaged new files, stage or delete them:\n$UNADDED_FILES\n"
    exit 1;
fi

MODIFIED_FILES=$(git diff --name-only)
MODIFIED_FILE_COUNT=$(echo -n "$MODIFIED_FILES" | wc -l)
if [ "$MODIFIED_FILE_COUNT" -ne 0 ]; then
    >&2 printf "Unstaged modified files, stage or revert them:\n$MODIFIED_FILES\n"
    exit 1;
fi

printf "Pre-commit Unit Tests...\n"
"$PROJECT_SCRIPTS_DIR/all_tests.sh"
ALL_TESTS_RESULT="$?"

if [ "$ALL_TESTS_RESULT" -ne 0 ]; then
    >&2 printf "Test failure, cannot commit.\n"
	exit 1;
fi

printf "Done.\n"
exit 0;
