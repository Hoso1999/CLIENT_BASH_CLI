cmake_minimum_required(VERSION 3.20)
project(command_communication)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BINARY_DIR "./build")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/network
    ${PROJECT_SOURCE_DIR}/include/command
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/network
    ${PROJECT_SOURCE_DIR}/src/command
)

file(GLOB server_SRCS
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/server.cpp"
    "${PROJECT_SOURCE_DIR}/src/network/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/command/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/network/*.c"
    "${PROJECT_SOURCE_DIR}/src/command/*.c"
)

file(GLOB client_SRCS
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/client.cpp"
    "${PROJECT_SOURCE_DIR}/src/network/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/command/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/network/*.c"
    "${PROJECT_SOURCE_DIR}/src/command/*.c"
)




add_executable(server ${server_SRCS})
add_executable(client ${client_SRCS})



