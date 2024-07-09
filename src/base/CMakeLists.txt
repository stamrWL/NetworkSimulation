file(GLOB BASE_SOURCES *.cpp)

message("BASE_SOURCES: ${BASE_SOURCES}")

add_library(BaseLib STATIC ${BASE_SOURCES})

set_target_properties(  BaseLib 
                        PROPERTIES
                        CXX_STANDARD 17
                        CXX_EXTENSIONS off
                        )


target_include_directories(BaseLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})