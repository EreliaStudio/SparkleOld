cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(Sparkle)

set(OSBitness 32)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(OSBitness 64)
endif()

set(ArchitectureSufix "${CMAKE_SYSTEM_NAME}/x${OSBitness}/${CMAKE_BUILD_TYPE}")

find_package(Vulkan REQUIRED)

if(VULKAN_FOUND)
    message(STATUS "Found Vulkan, including: ${Vulkan_INCLUDE_DIRS}")
    include_directories(${Vulkan_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Could not find Vulkan")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/archive/${ArchitectureSufix}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${ArchitectureSufix}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/executable/${ArchitectureSufix}")

function(SetupSparkleLibrary)

	include_directories(${CMAKE_SOURCE_DIR}/includes)

	file(GLOB SOURCE_FILES 
		"${CMAKE_SOURCE_DIR}/src/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/input/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/system/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/network/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/miscellaneous/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/miscellaneous/JSON/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/application/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/application/modules/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/widget/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/data_structure/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/design_pattern/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/iostream/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/math/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/threading/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/debug/*.cpp"

	)

if(UNIX)
	file(GLOB SYSTEM_DEPENDENT_SOURCE_FILES
		"${CMAKE_SOURCE_DIR}/src/linux/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/network/linux/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/linux/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/input/linux/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/application/modules/linux/*.cpp"
	)

elseif(WIN32)
	file(GLOB SYSTEM_DEPENDENT_SOURCE_FILES
		"${CMAKE_SOURCE_DIR}/src/windows/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/network/windows/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/windows/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/input/windows/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/application/modules/windows/*.cpp"
	)

endif()

	add_library(sparkle ${SOURCE_FILES} ${SYSTEM_DEPENDENT_SOURCE_FILES})
		
endfunction()

function(SetupPlayground)
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
endfunction()

SetupSparkleLibrary()
SetupPlayground()