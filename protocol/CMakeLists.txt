cmake_minimum_required(VERSION 2.6)

project(protocol)

set(CFLAG -O2 -DCOMPUTER -std=c++11)

include_directories ("${PROJECT_SOURCE_DIR}")

add_definitions(${CFLAG})

add_library(transmission SHARED transmission.cpp utils.cpp)

add_executable(protocol_generator ui.cpp)

target_link_libraries(protocol_generator transmission)
