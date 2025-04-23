cmake_minimum_required(VERSION 3.22)

project(threads)

set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)


set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(sort ${CMAKE_CURRENT_SOURCE_DIR}/sort.cc)


target_include_directories(sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
