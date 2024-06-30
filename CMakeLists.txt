cmake_minimum_required(VERSION 3.10)
project(Simulation CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER "C:/Users/stamr/scoop/apps/gcc/13.2.0/bin/g++.exe")

# set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# set(CMAKE_CUDA_COMPILER "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.3")
# set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
# set(CUDA_ARCHITECTURES 52)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

add_subdirectory("src")

add_executable(Simulation main.cpp)

target_link_libraries(Simulation PRIVATE BaseLib Simulation_)