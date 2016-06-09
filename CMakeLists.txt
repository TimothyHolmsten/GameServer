cmake_minimum_required(VERSION 3.0.2)
project(GameServer)

set(CMAKE_C_FLAGS "-std=c99 -D_XOPEN_SOURCE -D_POSIX_C_SOURCE=200809L -pthread")

file(GLOB SERVER_SRC src/*.c src/headers/*.h)

set(SOURCE_FILES ${SERVER_SRC})
add_executable(Server_Start ${SOURCE_FILES})