# BOOKON

Manage bookmarks and actions

## Building

## Build instructions
You will need the following tools :

- A recent compiler for C++
- vcpkg (https://vcpkg.io/en/)
- CMake

### Create and build the Project / Solution

    mkdir [build directory]
    cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake 
    cmake --build [build directory]
    cd [build directory]
    cpack -G DEB