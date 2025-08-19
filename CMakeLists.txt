# set the minimum cmake version
cmake_minimum_required(VERSION 3.15)
project(http_server)

set(TARGET ${PROJECT_NAME})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOUCE_DIR}/bin)

set( SOURCE_PATH "${PROJECT_SOURCE_DIR}/src" )
set( INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include" )

file( GLOB_RECURSE HPPS "${INCLUDE_PATH}/*.hpp" )
file( GLOB_RECURSE CPPS "${SOURCE_PATH}/*.cpp" )

# Export compile commands for LSP
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# adds source files
set(SOURCES
    src/main.cpp
    src/handle_client.cpp
    src/network.cpp
)

# Creates executable
add_executable(http_server ${CPPS} ${HPPS})

# include directories
target_include_directories( ${TARGET} PUBLIC ${INCLUDE_PATH} )
# target_include_directories(http_server PRIVATE "http_server/include")
