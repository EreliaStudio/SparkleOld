file(GLOB CURRENT_DIR_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SOURCE_FILES ${CURRENT_DIR_CPP_FILES})

add_subdirectory(push_constants)
add_subdirectory(storage)

set(SOURCE_FILES "${SOURCE_FILES}" PARENT_SCOPE)