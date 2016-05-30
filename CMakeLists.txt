cmake_minimum_required(VERSION 3.5)
project(GameServerTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SERVER_SRC src/*.c headers/*.h)

set(SOURCE_FILES ${SERVER_SRC})
add_executable(GameServerTest ${SOURCE_FILES})