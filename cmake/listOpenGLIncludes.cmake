function(listOpenGLIncludes INCLUDES_TABLE_VAR)
	set(OPENGL_INCLUDES_FILES
		"")

	# --- Specific Windows
	if (WIN32)
		set(OPENGL_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/windows/opengl/spk_graphical_api_surface.hpp - graphics"
			)

	# --- Specific Linux
	elseif(UNIX)
		set(OPENGL_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/linux/opengl/spk_graphical_api_surface.hpp - graphics"
			)

	endif()

	set(${INCLUDES_TABLE_VAR} ${${INCLUDES_TABLE_VAR}} ${OPENGL_INCLUDES_FILES} ${OPENGL_SYSTEM_DEPENDENT_INCLUDES_FILES} PARENT_SCOPE)

endfunction()