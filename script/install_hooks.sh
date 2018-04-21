#!/usr/bin/env bash

PROJECT_ROOT_DIR="$(git rev-parse --show-toplevel)"
PROJECT_SCRIPTS_DIR="$PROJECT_ROOT_DIR/script"
GIT_DIR="$(git rev-parse --git-dir)"

printf "Installing git hooks...\n"
ln -sf "$PROJECT_SCRIPTS_DIR/pre_commit.sh" "$GIT_DIR/hooks/pre-commit"
printf "Done.\n"
