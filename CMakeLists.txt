cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(Sparkle)

set(OSBitness 32)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()

set(ArchitectureSufix "${CMAKE_SYSTEM_NAME}/x${OSBitness}/${CMAKE_BUILD_TYPE}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/archive/${ArchitectureSufix}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${ArchitectureSufix}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/executable/${ArchitectureSufix}")

function(SetupSparkleLibrary)

    include_directories(${CMAKE_SOURCE_DIR}/includes)

    file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

    list(FILTER SOURCE_FILES EXCLUDE REGEX "${CMAKE_SOURCE_DIR}/src/cross_platform/.*")

    add_library(sparkle ${SOURCE_FILES})
    
endfunction()

function(SetupPlayground)
    set(SOURCE_FILES)
    
    file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/playground/src/*.cpp")

    add_executable(playground ${SOURCE_FILES})
    target_include_directories(playground PRIVATE ${CMAKE_SOURCE_DIR}/playground/includes)
    target_include_directories(playground PRIVATE ${CMAKE_SOURCE_DIR}/includes)
    target_link_libraries(playground sparkle)
	target_link_libraries(playground xcb)
endfunction()

SetupSparkleLibrary()
SetupPlayground()