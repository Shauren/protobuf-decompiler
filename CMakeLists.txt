cmake_minimum_required(VERSION 3.0)
project(protobuf_decompiler)

find_package(Protobuf)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
find_package(Boost 1.51 REQUIRED filesystem system)

include_directories(${PROTOBUF_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_executable(protobuf_decompiler
  main.cpp
  MetadataExtractor.cpp
  MetadataExtractor.h
  FilesystemMetadataExtractor.cpp
  FilesystemMetadataExtractor.h
  BinaryMetadataExtractor.cpp
  BinaryMetadataExtractor.h)

target_link_libraries(protobuf_decompiler ${PROTOBUF_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS protobuf_decompiler RUNTIME DESTINATION bin)
