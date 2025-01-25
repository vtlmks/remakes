#!/bin/bash

# Configuration based on argument
BUILD_TYPE=$1

if [ "$BUILD_TYPE" == "debug" ]; then
	CFLAGS="-mtune=native -g -O0 -msse4.1 -std=gnu11 -ffunction-sections -fdata-sections -Wall -Wextra -fwrapv -ffast-math -fno-trapping-math -Wno-unused-parameter -Wno-sign-compare -Wno-trigraphs -Wno-unused-variable -Wno-unused-const-variable -Wno-unused-function"
else
	CFLAGS="-mtune=native -O2 -msse4.1 -std=gnu11 -flto -ffunction-sections -fdata-sections -Wall -Wextra -fwrapv -ffast-math -fno-trapping-math -Wno-unused-parameter -Wno-sign-compare -Wno-trigraphs -Wno-unused-variable -Wno-unused-const-variable -Wno-unused-function"
fi
LDFLAGS="-Wl,--gc-sections"

# Make sure the compilation stops if any error happens.
set -e

# Pre-build tasks
../bin/shader2h 140 vertex_shader vertex_shader.glsl
../bin/shader2h 140 fragment_shader shader.h fragment_shader.glsl

# Build Linux version
echo "Building linux version ($BUILD_TYPE)"
../bin/ctime -begin .base_linux
gcc $CFLAGS base.c -o base -I../include -L../lib/linux64 -l:libglfw3.a -lX11 -lasound -lGL -lm -ldl -pthread
../bin/ctime -end .base_linux $?

# Build Windows version
echo "Building windows version ($BUILD_TYPE)"
../bin/ctime -begin .base_windows
x86_64-w64-mingw32-gcc $CFLAGS base.c -o base.exe -I../include -L../lib/win32 -l:libglfw3.a -lwinmm -lntdll -lgdi32 -luser32 -lopengl32 -ladvapi32 -limm32
../bin/ctime -end .base_windows $?
