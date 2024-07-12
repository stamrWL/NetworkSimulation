
add_subdirectory(base)


if(NOT MYMake_PYTHON)
    add_subdirectory(net)
endif()

add_library(Simulation_ STATIC Simulation.cpp ThreadPool.cpp)

set_target_properties(  Simulation_ 
                        PROPERTIES
                        CXX_STANDARD 17
                        CXX_EXTENSIONS off
                        )

target_include_directories(Simulation_ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(MYMake_PYTHON)

    target_link_libraries(Simulation_ BaseLib)

else()

    target_link_libraries(Simulation_ BaseLib NetLib)


endif()