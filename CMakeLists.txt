cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ProjectName "ES_2t_gen")

project("${ProjectName}")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
find_package(OpenGL REQUIRED)

add_executable("${ProjectName}"
	#src files
	src/main.cpp
	#header files
)