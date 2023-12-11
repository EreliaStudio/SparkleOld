file(GLOB CURRENT_DIR_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SOURCE_FILES ${CURRENT_DIR_CPP_FILES})

add_subdirectory(font)
add_subdirectory(frame)
add_subdirectory(pipeline)
add_subdirectory(surface)
add_subdirectory(texture)
add_subdirectory(viewport)

set(SOURCE_FILES "${SOURCE_FILES}" PARENT_SCOPE)