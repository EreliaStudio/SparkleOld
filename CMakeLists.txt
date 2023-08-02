cmake_minimum_required(VERSION 3.0.0)

include(cmake/listSourceFiles.cmake)

project(Sparkle
    VERSION 1.0
    DESCRIPTION "Graphical application library, using Vulkan and C++20"
    LANGUAGES CXX)

set(OSBitness 32 CACHE STRING "OS Bitness")
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64 CACHE STRING "OS Bitness" FORCE)
endif()

set(ArchitectureSufix "${CMAKE_SYSTEM_NAME}/x${OSBitness}/${CMAKE_BUILD_TYPE}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/archive/${ArchitectureSufix}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/${ArchitectureSufix}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/executable/${ArchitectureSufix}")

find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

listSourceFiles(SOURCE_FILES SYSTEM_DEPENDENT_SOURCE_FILES)

add_library(Sparkle STATIC ${SOURCE_FILES} ${SYSTEM_DEPENDENT_SOURCE_FILES})

target_include_directories(Sparkle
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
    INTERFACE
        $<INSTALL_INTERFACE:include>
)

target_compile_features(Sparkle PUBLIC cxx_std_20)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION includeTest
        )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

configure_file(cmake/${PROJECT_NAME}Config.cmake.in cmake/${PROJECT_NAME}Config.cmake)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/includes/
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

add_subdirectory(playground)