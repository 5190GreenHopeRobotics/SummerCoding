cmake_minimum_required(VERSION 2.6)

project(protocol)

add_library(transmission SHARED transmission.cpp)

include_directories ("${PROJECT_SOURCE_DIR}")

