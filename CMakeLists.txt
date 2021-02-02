cmake_minimum_required(VERSION 3.5.0)
# CMAKE version of Ubuntu 16.04 is 3.5.
project(WAV VERSION 1.0.0 LANGUAGES CXX)


set(SRC)
set(INCL)
set(LINKLIBS)
set(COMPILE_OPTION)

list(APPEND SRC 
	test.cpp
	
	WAV.h
)

add_executable(${PROJECT_NAME}  ${SRC})
target_include_directories(${PROJECT_NAME}	PRIVATE	${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )