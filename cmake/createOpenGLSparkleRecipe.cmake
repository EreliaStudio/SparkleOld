function(createOpenGLRecipe)
	message(STATUS "Creating OpenGL recipe")
	set(ProductName "SparkleOpenGL")

	include(cmake/listGenericSources.cmake)
	include(cmake/listOpenGLSources.cmake)

	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})

	listGenericSources(SOURCE_FILES)
	listOpenGLSources(SOURCE_FILES)

	add_library(${ProductName} STATIC ${SOURCE_FILES})

	if(UNIX)
		target_link_libraries(${ProductName} xcb)
	endif()

	target_include_directories(${ProductName}
		PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
		INTERFACE $<INSTALL_INTERFACE:include>
	)

	target_compile_definitions(${ProductName} PUBLIC GRAPHICAL_API=0)

	if(UNIX)
		target_link_libraries(${ProductName} OpenGL)
	else()
		target_link_libraries(${ProductName} OpenGL32.lib)
	endif()

	target_link_libraries(${ProductName} GLEW::GLEW)
endfunction()