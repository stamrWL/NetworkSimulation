cmake_minimum_required(VERSION 3.10)
project(Simulation CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -pthread  -lpthread")

add_subdirectory("src")

add_executable(Simulation main.cpp)

target_link_libraries(Simulation BaseLib Simulation_)