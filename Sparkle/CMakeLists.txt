cmake_minimum_required(VERSION 3.15.2)

set(ProductName "SparkleOpenGL")

list(APPEND CMAKE_PREFIX_PATH "C:/Program Files/glew")
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

set(INCLUDE_DIRECTORIES "")
set(SOURCE_FILES "")

add_subdirectory(includes)
add_subdirectory(src)

add_library(${ProductName} STATIC ${SOURCE_FILES})

if(UNIX)
	target_link_libraries(${ProductName} GL xcb X11 X11-xcb)
else()
    target_link_libraries(${ProductName} ws2_32 imagehlp)
endif()


target_include_directories(${ProductName} PUBLIC ${INCLUDE_DIRECTORIES})

target_compile_definitions(${ProductName} PUBLIC GRAPHICAL_API=0)

if(UNIX)
    target_link_libraries(${ProductName} OpenGL)
else()
    target_link_libraries(${ProductName} OpenGL32.lib)
endif()

target_link_libraries(${ProductName} GLEW::GLEW)