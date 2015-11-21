cmake_minimum_required(VERSION 2.6)

project(protocol)

set(CFLAG -O2)

include_directories ("${PROJECT_SOURCE_DIR}")

add_definitions(${CFLAG})

add_library(transmission SHARED transmission.cpp)

add_executable(testLib test.cpp)

target_link_libraries(testLib transmission)