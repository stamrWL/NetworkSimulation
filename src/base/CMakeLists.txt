file(GLOB BASE_SOURCES *.cpp)

add_library(BaseLib SHARED ${BASE_SOURCES})

target_include_directories(BaseLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})