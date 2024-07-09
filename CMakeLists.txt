cmake_minimum_required(VERSION 3.10)

project(Simulation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTEST_DEBUG")

find_package(asio CONFIG REQUIRED)

add_subdirectory("src")

add_executable(Simulation main.cpp)

target_link_libraries(Simulation PUBLIC Simulation_ asio::asio ws2_32)