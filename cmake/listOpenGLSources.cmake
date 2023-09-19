function(listOpenGLSources SOURCE_FILES_VAR)

	# --- Common to both Linux and Windows
	file(GLOB OPENGL_COMMON_SOURCE_FILES 
		"graphics/frame/opengl/*.cpp"
	)

	# --- Specific Windows
	if (WIN32)
		file(GLOB OPENGL_SPECIFIC_SOURCE_FILES
			"graphics/frame/opengl/windows/*.cpp"
		)

	# --- Specific Linux
	elseif(UNIX)
		file(GLOB OPENGL_SPECIFIC_SOURCE_FILES
		"graphics/frame/opengl/linux/*.cpp"
		)

	endif()

	set(${SOURCE_FILES_VAR} ${${SOURCE_FILES_VAR}} ${OPENGL_COMMON_SOURCE_FILES} ${OPENGL_SPECIFIC_SOURCE_FILES} PARENT_SCOPE)

endfunction()