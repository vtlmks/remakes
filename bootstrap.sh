#!/bin/bash

build_glfw_linux() {
	echo "Building GLFW for Linux..."
	pushd glfw || { echo "Failed to enter GLFW directory, have you checked out the submodules?"; exit 1; }
	mkdir -p build
	pushd build || { echo "Failed to enter build directory"; exit 1; }
	cmake .. -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_X11=ON -DGLFW_BUILD_WAYLAND=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS="-ffunction-sections -fdata-sections" -DCMAKE_EXE_LINKER_FLAGS="-Wl,--gc-sections"
	make -j"$(nproc)" || { echo "Failed to build GLFW for Linux"; exit 1; }
	mv src/*.a "../../lib/linux64" || { echo "Failed to move GLFW library files"; exit 1; }
	popd || { echo "Failed to return to the original directory"; exit 1; }
	rm -rf build
	popd || { echo "Failed to return to the original directory"; exit 1; }
	echo "GLFW built for Linux successfully."
}

build_glfw_windows() {
	echo "Building GLFW for Windows with MinGW..."
	pushd glfw || { echo "Failed to enter GLFW directory, have you checked out the submodules?"; exit 1; }
	mkdir -p build
	pushd build || { echo "Failed to enter build directory"; exit 1; }
	cmake .. -DCMAKE_TOOLCHAIN_FILE=../CMake/x86_64-w64-mingw32.cmake -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS="-ffunction-sections -fdata-sections" -DCMAKE_EXE_LINKER_FLAGS="-Wl,--gc-sections"
	make -j"$(nproc)" || { echo "Failed to build GLFW for Windows with MinGW"; exit 1; }
	mv src/*.a "../../lib/win32" || { echo "Failed to move GLFW library files"; exit 1; }
	popd || { echo "Failed to return to the original directory"; exit 1; }
	rm -rf build
	popd || { echo "Failed to return to the original directory"; exit 1; }
	echo "GLFW built for Windows with MinGW successfully."
}

# Function to copy include files
copy_include_files() {
	echo "Copying include files..."
	cp -r glfw/include/* include || { echo "Failed to copy include directory"; exit 1; }
	echo "Include files copied successfully."
}

# Function to build tools
build_tools() {
	echo "Building tools..."
	pushd ./tools || { echo "Failed to enter tools directory"; exit 1; }
	./build.sh || { echo "Failed to build tools"; exit 1; }
	popd || { echo "Failed to return to the original directory"; exit 1; }
	echo "Tools built successfully."
}

# Main script
mkdir -p lib/{win32,linux64}
build_glfw_linux
build_glfw_windows
copy_include_files
build_tools
