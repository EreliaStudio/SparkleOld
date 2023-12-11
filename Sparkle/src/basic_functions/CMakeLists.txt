file(GLOB CURRENT_DIR_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SOURCE_FILES ${CURRENT_DIR_CPP_FILES})

if(UNIX AND NOT APPLE)
	add_subdirectory(linux)
elseif(WIN32)
	add_subdirectory(windows)
endif()

set(SOURCE_FILES "${SOURCE_FILES}" PARENT_SCOPE)