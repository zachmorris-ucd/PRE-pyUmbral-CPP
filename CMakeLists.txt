cmake_minimum_required(VERSION 3.8)
project(PRE_CPP)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp main.h)

add_executable(PRE_CPP ${SOURCE_FILES})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(PRE_CPP ${PYTHON_LIBRARIES})
