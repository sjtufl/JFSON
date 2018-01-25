cmake_minimum_required(VERSION 3.8)
project(JFSON)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp JFSON.cpp JFSON.h)
add_executable(JFSON ${SOURCE_FILES})