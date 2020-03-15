cmake_minimum_required(VERSION 3.0.0)
project(pyalphaexpansion)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(pybind11 REQUIRED)

set(SOURCES
    bindings.cpp)

pybind11_add_module(pyalphaexpansion ${SOURCES})

find_library(
    ALPHAEXP_LIB
    AlphaExpansion
    HINTS ../ ../../
    PATH_SUFFIXES build
)

message(STATUS ${ALPHAEXP_LIB})

target_include_directories(pyalphaexpansion PRIVATE ../)
target_link_libraries(pyalphaexpansion PRIVATE ${ALPHAEXP_LIB})
target_compile_options(pyalphaexpansion PRIVATE -fPIC)