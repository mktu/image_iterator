cmake_minimum_required(VERSION 2.8.12)
project(image_iterator_sample CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -pg")

include_directories(../ImageLib)

add_executable(image_iterator_sample sample.cpp)