cmake_minimum_required(VERSION 3.21)
project(WAV VERSION 1.0.0 LANGUAGES CXX)


set(SRC)
set(INCL)
set(LINKLIBS)
set(COMPILE_OPTION)

list(APPEND SRC 
	WAV.h
	WAV.cpp
	WAVw_wrap.cpp
)

if(APPLE)
 set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

add_library(${PROJECT_NAME}  SHARED ${SRC})
target_include_directories(${PROJECT_NAME}	PRIVATE	${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )

add_executable(test_WAV test.cpp)
if(MSVC)
	target_link_libraries(test_WAV PRIVATE ${CMAKE_CURRENT_LIST_DIR}/build/Release/${PROJECT_NAME}.lib)
elseif(APPLE)
	target_link_libraries(test_WAV PRIVATE ${CMAKE_CURRENT_LIST_DIR}/build/lib${PROJECT_NAME}.dylib)
else()
	target_link_libraries(test_WAV PRIVATE ${CMAKE_CURRENT_LIST_DIR}/build/lib${PROJECT_NAME}.so)
endif()
