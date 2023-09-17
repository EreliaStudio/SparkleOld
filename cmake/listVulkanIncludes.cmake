function(listVulkanIncludes INCLUDES_TABLE_VAR)
	set(VULKAN_INCLUDES_FILES
		"")

	# --- Specific Windows
	if (WIN32)
		set(VULKAN_SYSTEM_DEPENDENT_INCLUDES_FILES
		"${CMAKE_SOURCE_DIR}/includes/graphics\surface\windows\vulkan;includes/graphics\surface\windows\vulkan"
		"${CMAKE_SOURCE_DIR}/includes/graphics\surface\windows\vulkan\spk_graphical_api_surface.hpp;includes/graphics\surface\windows\vulkan\spk_graphical_api_surface.hpp"
		)

	# --- Specific Linux
	elseif(UNIX)
		set(VULKAN_SYSTEM_DEPENDENT_INCLUDES_FILES
		"${CMAKE_SOURCE_DIR}/includes/graphics\surface\linux\vulkan;includes/graphics\surface\linux\vulkan"
		"${CMAKE_SOURCE_DIR}/includes/graphics\surface\linux\vulkan\spk_graphical_api_surface.hpp;includes/graphics\surface\linux\vulkan\spk_graphical_api_surface.hpp"
		)

	endif()

	set(${p_INCLUDES_TABLE_VAR} ${${p_INCLUDES_TABLE_VAR}} ${VULKAN_INCLUDES_FILES} ${VULKAN_SYSTEM_DEPENDENT_INCLUDES_FILES} PARENT_SCOPE)

endfunction()