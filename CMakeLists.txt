cmake_minimum_required(VERSION 3.5)
project(GameServerTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c masterserver.c masterserver.h)
add_executable(GameServerTest ${SOURCE_FILES})