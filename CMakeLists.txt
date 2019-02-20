cmake_minimum_required(VERSION 3.8)
project(JFSON)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-Wall
                -Werror
                #-Woverloaded-virtual
                )

set(SOURCE_FILES main.cpp JFSON.cpp JFSON.h JsonValue.cc JsonValue.h noncopyable.h Document.h Exception.h)
add_executable(JFSON ${SOURCE_FILES})

add_subdirectory(src)