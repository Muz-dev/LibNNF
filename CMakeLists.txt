cmake_minimum_required(VERSION 3.30)
project(LibNNF)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE src
    lib/*.cpp
    main.cpp
)

add_executable(LibNNF ${src})

target_include_directories(LibNNF PRIVATE lib/)