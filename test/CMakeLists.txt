message(STATUS "This is a status message from test.")


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

if (PROTO_SRCS)
    message(STATUS "Generated source files: ${PROTO_SRCS}")
else()
    message(FATAL_ERROR "No source files generated from proto files")
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})



set(TEST_HELPERS Test_Helpers.cpp ${PROTO_SRCS} ${PROTO_HDRS}) 

add_executable(Test_Helpers ${TEST_HELPERS}) 

target_link_libraries(Test_Helpers ${GTEST_BOTH_LIBRARIES} pthread ${Protobuf_LIBRARIES})

target_include_directories(Test_Helpers PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/protobuf-parser)



set(TEST_DELIMITED_MESSAGES_STREAM_PARSER_FILES Test_DelimitedMessagesStreamParser.cpp ${PROTO_SRCS} ${PROTO_HDRS}) 

add_executable(Test_DelimitedMessagesStreamParser ${TEST_DELIMITED_MESSAGES_STREAM_PARSER_FILES}) 

target_link_libraries(Test_DelimitedMessagesStreamParser ${GTEST_BOTH_LIBRARIES} pthread ${Protobuf_LIBRARIES})

target_include_directories(Test_DelimitedMessagesStreamParser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/protobuf-parser)