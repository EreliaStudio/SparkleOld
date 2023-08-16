cmake_minimum_required(VERSION 3.0.0)

include(cmake/listSourceFiles.cmake)

project(Sparkle
    VERSION 1.0
    DESCRIPTION "Graphical application library, using Vulkan and C++20"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(OSBitness 32)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()

set(ArchitectureSufix "${CMAKE_SYSTEM_NAME}/x${OSBitness}/${CMAKE_BUILD_TYPE}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/archive/${ArchitectureSufix}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/${ArchitectureSufix}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/executable/${ArchitectureSufix}")

find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

listSourceFiles(SOURCE_FILES SYSTEM_DEPENDENT_SOURCE_FILES)

add_library(Sparkle STATIC ${SOURCE_FILES} ${SYSTEM_DEPENDENT_SOURCE_FILES})

if (UNIX)
	target_link_libraries(Sparkle xcb)
endif()

target_include_directories(Sparkle
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
    INTERFACE $<INSTALL_INTERFACE:include>
)

add_subdirectory(playground)

setupLibraryInstallation()