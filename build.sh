#!/bin/bash

set -xe

FRAMEWORK_FLAGS="-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL"
CFLAGS="-Wall -Wextra -pedantic -std=c11 -ggdb"
clang c-sweep.c deps/libraylib.a -o build/c-sweep $CFLAGS $FRAMEWORK_FLAGS -Ideps -Isrc
