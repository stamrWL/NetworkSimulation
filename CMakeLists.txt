cmake_minimum_required(VERSION 3.10)

project(Simulation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(MYMake_PYTHON TRUE)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(asio CONFIG REQUIRED)

add_subdirectory("src")

if(MYMake_PYTHON)

    message("Make Python version")

    set(PYTHON_EXECUTABLE "C:\\Users\\stamr\\AppData\\Local\\Programs\\Python\\Python37\\python.exe")

    set(PYTHON_INCLUDE_DIRS "C:\\Users\\stamr\\AppData\\Local\\Programs\\Python\\Python37\\include\\Python.h")

    add_subdirectory("src/extern/pybind11")

    pybind11_add_module(Simulation bind.cpp)

    target_link_libraries(Simulation PUBLIC Simulation_ )

else()
    message("Make C++ version")

    add_executable(Simulation main.cpp)
    
    target_link_libraries(Simulation PUBLIC Simulation_ asio::asio ws2_32)

endif()
