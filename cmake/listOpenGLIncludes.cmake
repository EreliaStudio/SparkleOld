function(listOpenGLIncludes INCLUDES_TABLE_VAR)
	set(OPENGL_INCLUDES_FILES
		"${CMAKE_SOURCE_DIR}/includes/graphics/surface/opengl/windows/spk_graphical_api_surface.hpp - graphics"
		"${CMAKE_SOURCE_DIR}/includes/graphics/pipeline/opengl/spk_graphical_api_pipeline.hpp - graphics"
		"${CMAKE_SOURCE_DIR}/includes/graphics/pipeline/opengl/spk_graphical_api_pipeline_basic_functions.hpp - graphics"
		)

	# --- Specific Windows
	if (WIN32)
		set(OPENGL_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/opengl/windows/spk_graphical_api_surface.hpp - graphics"
			)

	# --- Specific Linux
	elseif(UNIX)
		set(OPENGL_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/opengl/linux/spk_graphical_api_surface.hpp - graphics"
			)

	endif()

	set(${INCLUDES_TABLE_VAR} ${${INCLUDES_TABLE_VAR}} ${OPENGL_INCLUDES_FILES} ${OPENGL_SYSTEM_DEPENDENT_INCLUDES_FILES} PARENT_SCOPE)

endfunction()