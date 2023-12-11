file(GLOB CURRENT_DIR_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SOURCE_FILES ${CURRENT_DIR_CPP_FILES})

add_subdirectory(gpu_object)
add_subdirectory(object)
add_subdirectory(uniform)

set(SOURCE_FILES "${SOURCE_FILES}" PARENT_SCOPE)