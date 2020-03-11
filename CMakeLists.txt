cmake_minimum_required(VERSION 3.0.0)
project(AlphaExpansion VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED)

file(GLOB SOURCES
    AlphaExpansion.cpp
    AlphaExpansion.h
    pushrelabel.h
    graph.h)

add_library(AlphaExpansion ${SOURCES})

add_executable(Main autre_main.cpp)
add_executable(SmokeTest main.cpp)
target_link_libraries(Main AlphaExpansion)
target_link_libraries(SmokeTest AlphaExpansion)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
