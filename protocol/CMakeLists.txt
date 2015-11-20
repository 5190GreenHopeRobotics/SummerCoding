cmake_minimum_required(VERSION 2.6)

project(protocol)

set(CFLAG -Os)

include_directories ("${PROJECT_SOURCE_DIR}")

add_definitions(${CFLAG})

add_library(transmission SHARED transmission.cpp)
