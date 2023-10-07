cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(Playground
	VERSION 1.0
	DESCRIPTION "A template for Project using Sparkle library."
	LANGUAGES CXX)

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(Playground ${SRC_FILES})

target_include_directories(Playground PRIVATE "${CMAKE_SOURCE_DIR}/includes" "${CMAKE_CURRENT_SOURCE_DIR}/includes")
target_link_directories(Playground PRIVATE "${CMAKE_SOURCE_DIR}/archive")

if(GraphicalAPI STREQUAL "OpenGL")
	target_link_libraries(Playground SparkleOpenGL)
else()
	target_link_libraries(Playground Sparkle)
endif()