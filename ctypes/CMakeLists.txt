cmake_minimum_required(VERSION 3.5)
project(WAV)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

set(INCL)
set(LINKLIBS)
set(COMPILE_OPTION)


list(APPEND INCL ../)

list(APPEND LINKLIBS
    m
)
list(APPEND COMPILE_OPTION
)

message(${INCL})

set (SOURCE WAV_wrapper.cpp)
add_library(WAV MODULE ${SOURCE}) 
#target_include_directories(${PROJECT_NAME} private ${INCL})
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )