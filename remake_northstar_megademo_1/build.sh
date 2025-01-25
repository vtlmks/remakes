#!/bin/bash

# Configuration based on argument
BUILD_TYPE=$1

if [ "$BUILD_TYPE" == "debug" ]; then
	CFLAGS="-mtune=native -g -O0 -msse4.1 -std=gnu11 -ffunction-sections -fdata-sections -Wall -Wextra -fwrapv -ffast-math -fno-trapping-math -Wno-unused-parameter -Wno-sign-compare -Wno-trigraphs -Wno-unused-variable -Wno-unused-const-variable -Wno-unused-function"
else
	CFLAGS="-mtune=native -O2 -msse4.1 -std=gnu11 -flto -ffunction-sections -fdata-sections -Wall -Wextra -fwrapv -ffast-math -fno-trapping-math -Wno-unused-parameter -Wno-sign-compare -Wno-trigraphs -Wno-unused-variable -Wno-unused-const-variable -Wno-unused-function"
fi
LDFLAGS="-Wl,--gc-sections"

# Rebuild assets every time we compile
rm -rf data
mkdir -p data/p{1,2,3,4,5,6,7,8}
pushd org_assets > /dev/null
./process.sh
popd > /dev/null

# make sure the <basecode> shaders are up to date if you are experimenting with them.
pushd ../base > /dev/null
../bin/shader2h 140 vertex_shader vertex_shader.glsl
../bin/shader2h 140 fragment_shader shader.h fragment_shader.glsl
popd > /dev/null

# Make sure the compilation stops if any error happens.
set -e

# Build Linux version
../bin/ctime -begin .remake_linux
gcc $CFLAGS remake.c -o remake -I../include -I../base -L../lib/linux64 $LDFLAGS -l:libglfw3.a -lX11 -lasound -lGL -lm -ldl -pthread
../bin/ctime -end .remake_linux $?

# Build Windows version
../bin/ctime -begin .remake_windows
x86_64-w64-mingw32-gcc $CFLAGS remake.c -o remake.exe -mwindows -I../include -I../base -L../lib/win32 $LDFLAGS -l:libglfw3.a -lwinmm -lntdll -lgdi32 -luser32 -lopengl32 -ladvapi32 -limm32
../bin/ctime -end .remake_windows $?

#
# Since we compile using a unity-build vscode does not understand how to get
# hold of prototypes/signatures and variables from other places in the code,
# so we have to use ctags.
#
if command -v ctags >/dev/null 2>&1; then
	gcc -E remake.c -I../include -I../base -Wno-trigraphs > ctags.c
	ctags ctags.c
	rm ctags.c
fi

