cmake_minimum_required(VERSION 2.6)
project(protobuf_decompiler)

find_package(Protobuf)

include_directories(${PROTOBUF_INCLUDE_DIRS})

add_executable(protobuf_decompiler main.cpp)

target_link_libraries(protobuf_decompiler ${PROTOBUF_LIBRARIES})

install(TARGETS protobuf_decompiler RUNTIME DESTINATION bin)
