file(GLOB CURRENT_DIR_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SOURCE_FILES ${CURRENT_DIR_CPP_FILES})

add_subdirectory(abstract)

if (GraphicalAPI STREQUAL "OpenGL")
    add_subdirectory(opengl)
elseif (GraphicalAPI STREQUAL "Vulkan")
    add_subdirectory(vulkan)
endif()

set(SOURCE_FILES "${SOURCE_FILES}" PARENT_SCOPE)