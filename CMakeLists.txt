cmake_minimum_required(VERSION 3.10)

# Project name
project(JsCompiler)

# Set the C++ standard to C++11, C++14, or C++17, depending on your needs
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories (add the 'include' directory)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files from the 'include' directory
set(SOURCES
    include/principal.cpp           # Main file
    include/analizador_lexico.cpp   # Source file inside the include directory
)

# Add executable
add_executable(JsCompiler ${SOURCES})

