# README

## Prerequisites
To build and run the project, you need the following:

1. **WSL2 (Windows Subsystem for Linux)** installed on your Windows machine.
   - Note: Instructions for setting up WSL2 are not provided here.
2. **GCC** and **MinGW-w64** installed within your WSL2 environment.

## Build Instructions

1. Clone this repository and navigate to the root directory.
2. Run the `./bootstrap.sh` script to prepare the environment and build GLFW as a static library for both linux and windows.
   ```bash
   ./bootstrap.sh
   ```
3. Navigate to the `remake_*` directory of your choice.
4. Run the `build.sh` script to build the project.
   ```bash
   ./build.sh
   ```

## Output
- After the build completes, the following files will be generated in the same directory:
  - `remake` (linux version)
  - `remake.exe` (windows version)

You can now use the built binaries as needed.

