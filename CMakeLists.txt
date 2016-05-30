cmake_minimum_required(VERSION 3.5)
project(GameServerTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SERVER_SRC src/*.c src/headers/*.h)

set(SOURCE_FILES ${SERVER_SRC} src/server.c src/headers/server.h)
add_executable(GameServerTest ${SOURCE_FILES})