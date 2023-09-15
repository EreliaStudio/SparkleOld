function(listOpenGLSources SOURCE_FILES_VAR)

	# --- Common to both Linux and Windows
	file(GLOB OPENGL_COMMON_SOURCE_FILES 
		"${CMAKE_SOURCE_DIR}/src/graphics/window/opengl/*.cpp"
	)

	# --- Specific Windows
	if (WIN32)
		file(GLOB OPENGL_SPECIFIC_SOURCE_FILES
			"${CMAKE_SOURCE_DIR}/src/graphics/window/opengl/windows/*.cpp"
		)

	# --- Specific Linux
	elseif(LINUX)
		file(GLOB OPENGL_SPECIFIC_SOURCE_FILES
			"${CMAKE_SOURCE_DIR}/src/graphics/window/opengl/linux/*.cpp"
		)

	endif()

	set(${SOURCE_FILES_VAR} ${${SOURCE_FILES_VAR}} ${OPENGL_COMMON_SOURCE_FILES} ${OPENGL_SPECIFIC_SOURCE_FILES} PARENT_SCOPE)

endfunction()