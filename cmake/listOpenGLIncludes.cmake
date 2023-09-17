function(listOpenGLIncludes INCLUDES_TABLE_VAR)
	set(OPENGL_INCLUDES_FILES
		"")

	# --- Specific Windows
	if (WIN32)
		set(OPENGL_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/windows/opengl/spk_graphical_api_surface.hpp;includes/graphics/surface/windows/opengl/spk_graphical_api_surface.hpp"
			)

	# --- Specific Linux
	elseif(UNIX)
		set(OPENGL_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/linux/opengl/spk_graphical_api_surface.hpp;includes/graphics/surface/linux/opengl/spk_graphical_api_surface.hpp"
			)

	endif()

	set(${p_INCLUDES_TABLE_VAR} ${${p_INCLUDES_TABLE_VAR}} ${OPENGL_INCLUDES_FILES} ${OPENGL_SYSTEM_DEPENDENT_INCLUDES_FILES} PARENT_SCOPE)

endfunction()