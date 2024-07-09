# file(GLOB NET_SOURCES *.cpp)

# if(CMAKE_SYSTEM_NAME MATCHES "Windows")
#     set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
#     message(STATUS "Building on Windows")
# endif()

# find_package(asio CONFIG REQUIRED)
# target_link_libraries(NetLib PRIVATE asio::asio)

add_library(NetLib STATIC usst_Message.cpp Stamr_client.cpp)

set_target_properties(  NetLib 
                        PROPERTIES
                        CXX_STANDARD 17
                        CXX_EXTENSIONS off
                        )

target_include_directories(NetLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})