cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "sparkle")

project(${PROJECT_NAME} VERSION 1.0)

add_subdirectory(playground)
add_dependencies(sparkle_test ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

include_directories(includes)

file(GLOB SRC_SOURCES "src/*.cpp")
file(GLOB STRUCTURE_SOURCES "src/structure/*.cpp")
file(GLOB NETWORK_SOURCES "src/network/*.cpp")

if(WIN32)
    file(GLOB SYSTEM_SOURCES "src/windows/*.cpp")
elseif(UNIX)
    file(GLOB SYSTEM_SOURCES "src/linux/*.cpp")
endif()

set(SOURCES ${SRC_SOURCES} ${STRUCTURE_SOURCES} ${NETWORK_SOURCES} ${SYSTEM_SOURCES})

add_library(${PROJECT_NAME} ${SOURCES})