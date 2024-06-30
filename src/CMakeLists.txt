
add_subdirectory(base)

add_library(Simulation_ Simulation.cpp ThreadPool.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive")

target_include_directories(Simulation_ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Simulation_ PRIVATE BaseLib)