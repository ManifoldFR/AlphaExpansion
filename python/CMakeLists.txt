cmake_minimum_required(VERSION 3.0.0)
find_package(pybind11 REQUIRED)
project(pyalphaexpansion)

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