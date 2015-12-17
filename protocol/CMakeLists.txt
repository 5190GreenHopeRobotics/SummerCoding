cmake_minimum_required(VERSION 2.6)

project(protocol)


include_directories ("${PROJECT_SOURCE_DIR}")


add_executable(protocol_generator ui.cpp utils.cpp transmission.cpp)
