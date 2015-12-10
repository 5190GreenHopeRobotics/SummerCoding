cmake_minimum_required(VERSION 2.6)

project(protocol)


include_directories ("${PROJECT_SOURCE_DIR}")


add_executable(protocol_generator ui.cpp utils.cpp transmission.cpp)

set(needed_features
    cxx_delegating_constructors
    cxx_rvalue_references)
target_compile_features(protocol_generator PRIVATE ${needed_features})
