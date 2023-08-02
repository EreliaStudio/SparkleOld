cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(Sparkle)

set(OSBitness 32)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(OSBitness 64)
endif()

set(ArchitectureSufix "${CMAKE_SYSTEM_NAME}/x${OSBitness}/${CMAKE_BUILD_TYPE}")

find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/archive/${ArchitectureSufix}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${ArchitectureSufix}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/executable/${ArchitectureSufix}")

set(SOURCE_FILES)
	
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/playground/src/*.cpp")

add_executable(playground ${SOURCE_FILES})
target_include_directories(playground PRIVATE ${CMAKE_SOURCE_DIR}/playground/includes)
target_include_directories(playground PRIVATE ${CMAKE_SOURCE_DIR}/includes)
target_link_libraries(playground sparkle)

if(VULKAN_FOUND)
	message(STATUS "Found Vulkan, linking directory: ${Vulkan_LIBRARY_DIRS}")
	target_link_libraries(playground ${Vulkan_LIBRARIES})
else()
	message(FATAL_ERROR "Could not find Vulkan")
endif()

if(UNIX)
	target_link_libraries(playground xcb)
	target_link_libraries(playground pthread)
elseif(WIN32)
	target_link_libraries(playground ws2_32)
endif()