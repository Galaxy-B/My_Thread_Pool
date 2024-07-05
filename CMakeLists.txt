cmake_minimum_required(VERSION 3.15)
project(main)

set(CMAKE_CXX_STANDARD 17)

# set makefile generator
set(CMAKE_GENERATOR "MinGW Makefiles")
set(CMAKE_MAKE_PROGRAM "D:\\vscode\\mingw64\\bin\\make")

# don't generate clangd commands
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

# enable debug function
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# redirect the build output to a subdirectory of build
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# redirect the binary program to output directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)

include_directories(include)

aux_source_directory(src SRC_LIST)

add_executable(main ${SRC_LIST})