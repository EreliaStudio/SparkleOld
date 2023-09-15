cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(Playground
    VERSION 1.0
    DESCRIPTION "A template for Project using Sparkle library."
    LANGUAGES CXX)

find_package(Sparkle REQUIRED)

add_executable(Playground "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")