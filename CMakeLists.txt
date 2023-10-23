cmake_minimum_required(VERSION 3.26)
project(file_encrypting)

set(CMAKE_CXX_STANDARD 17)

add_library(caesar SHARED caesar.cpp)

set_target_properties(caesar PROPERTIES PREFIX "")
set_target_properties(caesar PROPERTIES SUFFIX ".dll")

target_compile_definitions(caesar PRIVATE CAESAR_EXPORTS)

add_executable(main main.cpp)