cmake_minimum_required(VERSION 3.0.0)

include(cmake/listSourceFiles.cmake)

project(Sparkle
    VERSION 1.0
    DESCRIPTION "Graphical application library, using Vulkan and C++20"
    LANGUAGES CXX)


find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

listSourceFiles(SOURCE_FILES SYSTEM_DEPENDENT_SOURCE_FILES)

add_library(Sparkle STATIC ${SOURCE_FILES} ${SYSTEM_DEPENDENT_SOURCE_FILES})

target_include_directories(
    Sparkle
	PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(Sparkle PUBLIC cxx_std_20)
