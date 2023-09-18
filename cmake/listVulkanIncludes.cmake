function(listVulkanIncludes INCLUDES_TABLE_VAR)
	set(VULKAN_INCLUDES_FILES
	"${CMAKE_SOURCE_DIR}/includes/graphics/surface/vulkan/spk_graphical_api_surface.hpp - graphics")

	# --- Specific Windows
	if (WIN32)
		set(VULKAN_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/vulkan/windows/spk_graphical_api_surface.hpp - graphics"
			)

	# --- Specific Linux
	elseif(UNIX)
		set(VULKAN_SYSTEM_DEPENDENT_INCLUDES_FILES
				"${CMAKE_SOURCE_DIR}/includes/graphics/surface/vulkan/linux/spk_graphical_api_surface.hpp - graphics"
			)

	endif()

	set(${INCLUDES_TABLE_VAR} ${${INCLUDES_TABLE_VAR}} ${VULKAN_INCLUDES_FILES} ${VULKAN_SYSTEM_DEPENDENT_INCLUDES_FILES} PARENT_SCOPE)

endfunction()