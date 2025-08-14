# set the minimum cmake version
cmake_minimum_required(VERSION 3.15)
project(http_server)

# adds source files
set(SOURCES
    src/main.cpp
)

# Creates executable
add_executable(http_server ${SOURCES})
