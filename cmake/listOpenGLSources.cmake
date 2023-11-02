function(listOpenGLSources SOURCE_FILES_VAR)

	# --- Common to both Linux and Windows
	file(GLOB OPENGL_COMMON_SOURCE_FILES 
		"${CMAKE_SOURCE_DIR}/src/graphics/surface/opengl/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/frame/opengl/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/texture/opengl/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/pipeline/opengl/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/pipeline/opengl/object/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/pipeline/opengl/uniform/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/pipeline/opengl/gpu_object/*.cpp"
	)

	# --- Specific Windows
	if (WIN32)
		file(GLOB OPENGL_SPECIFIC_SOURCE_FILES
			"${CMAKE_SOURCE_DIR}/src/graphics/frame/opengl/windows/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/graphics/surface/opengl/windows/*.cpp"
		)

	# --- Specific Linux
	elseif(UNIX)
		file(GLOB OPENGL_SPECIFIC_SOURCE_FILES
		"${CMAKE_SOURCE_DIR}/src/graphics/frame/opengl/linux/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/surface/opengl/linux/*.cpp"
		)

	endif()

	set(${SOURCE_FILES_VAR} ${${SOURCE_FILES_VAR}} ${OPENGL_COMMON_SOURCE_FILES} ${OPENGL_SPECIFIC_SOURCE_FILES} PARENT_SCOPE)

endfunction()