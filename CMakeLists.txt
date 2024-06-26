cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER "~/scoop/apps/gcc/13.2.0")

# set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# set(CMAKE_CUDA_COMPILER "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.3")
# set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
# set(CUDA_ARCHITECTURES 52)

project(CUDATEST1 LANGUAGES CXX)

add_executable(main main.cpp)
