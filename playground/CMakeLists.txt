cmake_minimum_required(VERSION 3.10)

if(WIN32)
    set(PROJECT_NAME "sparkle_test.exe")
elseif(UNIX)
    set(PROJECT_NAME "sparkle_test")
endif()

project(${PROJECT_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(includes ../includes)

file(GLOB SRC_SOURCES "src/*.cpp")

set(SOURCES ${SRC_SOURCES})

add_executable(${PROJECT_NAME} ${SOURCES})