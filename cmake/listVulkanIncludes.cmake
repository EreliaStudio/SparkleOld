function(listVulkanIncludes INCLUDES_TABLE_VAR)
	set(VULKAN_INCLUDES_FILES
	"${CMAKE_SOURCE_DIR}/includes/graphics/surface/vulkan/spk_device.hpp - graphics/surface/vulkan"
	"${CMAKE_SOURCE_DIR}/includes/graphics/surface/vulkan/spk_swapchain.hpp - graphics/surface/vulkan"
	"${CMAKE_SOURCE_DIR}/includes/graphics/surface/vulkan/spk_renderer.hpp - graphics/surface/vulkan"
	"${CMAKE_SOURCE_DIR}/includes/graphics/surface/vulkan/spk_surface.hpp - graphics"
	"${CMAKE_SOURCE_DIR}/includes/graphics/pipeline/vulkan/spk_pipeline.hpp - graphics"
	"${CMAKE_SOURCE_DIR}/includes/graphics/pipeline/vulkan/spk_pipeline_basic_functions.hpp - graphics")

	# --- Specific Windows
	if (WIN32)
		set(VULKAN_SYSTEM_DEPENDENT_INCLUDES_FILES
				""
			)

	# --- Specific Linux
	elseif(UNIX)
		set(VULKAN_SYSTEM_DEPENDENT_INCLUDES_FILES
				""
			)

	endif()

	set(${INCLUDES_TABLE_VAR} ${${INCLUDES_TABLE_VAR}} ${VULKAN_INCLUDES_FILES} ${VULKAN_SYSTEM_DEPENDENT_INCLUDES_FILES} PARENT_SCOPE)

endfunction()