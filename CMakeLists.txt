cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ProjectName "escamgen")
set(Subsystem "CONSOLE")

project("${ProjectName}" VERSION 0.1)
set(CMAKE_CXX_STANDARD 23)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(filters)

add_subdirectory(dependencies)

#app executable
add_executable("${ProjectName}"
        #src files
        src/main.cpp
        src/generator.cpp
        #header files
        include/generator.hpp
        include/units.hpp
)

target_include_directories("${ProjectName}" PRIVATE include dependencies/include dependencies/submodules)