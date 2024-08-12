message(STATUS "This is a status message from protocol.")



# Укажите путь к вашим .proto файлам
set(PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/message.proto CACHE PATH "Path to the file") 



#add_executable(blank test.cpp ${PROTO_SRCS} ${PROTO_HDRS})