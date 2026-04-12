#!/bin/bash

# fmt.sh - format all .c, .h, and meson.build files in the project.
# This should be run before any commit.

cd .. # move up to root

echo "Formating C files..."
find . -type d -name "build" -prune -o -type f -name "*.c" -print0 -o -type f -name "*.h" -print0 | xargs -0 clang-format -i

echo "Formatting Meson files..."
find . -type d -name "build" -prune -o -type f -name "meson.build" -print0 | xargs -0 -I {} meson format -i {}

echo "Done"
