cmake_minimum_required(VERSION 3.10)

# if(CMAKE_SYSTEM_NAME MATCHES "Windows")
#     set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
#     message(STATUS "Building on Windows")
# endif()

project(Simulation CXX)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -pthread  -lpthread -DTEST_DEBUG")

add_subdirectory("src")

add_executable(Simulation main.cpp)

target_link_libraries(Simulation Simulation_ NetLib)