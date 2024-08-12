cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)
set(CMAKE_CXX_STANDARD 11)
project(protobuf-parser VERSION 1.0.0 DESCRIPTION "Разбор потока length-prefixed Protobuf сообщений")

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/protobuf-parser)

enable_testing()
add_subdirectory(src/protocol)
add_subdirectory(test)

message(STATUS "This is a status message from main.")