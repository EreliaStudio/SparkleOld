function(listVulkanSources SOURCE_FILES_VAR)

	# --- Common to both Linux and Windows
	file(GLOB VULKAN_COMMON_SOURCE_FILES 
		""
	)

	# --- Specific Windows
	if (WIN32)
		file(GLOB VULKAN_SPECIFIC_SOURCE_FILES
			""
		)

	# --- Specific Linux
	elseif(LINUX)
		file(GLOB VULKAN_SPECIFIC_SOURCE_FILES
			""
		)

	endif()

	set(${SOURCE_FILES_VAR} ${${SOURCE_FILES_VAR}} ${VULKAN_COMMON_SOURCE_FILES} PARENT_SCOPE)
	list(APPEND ${SOURCE_FILES_VAR} ${VULKAN_SPECIFIC_SOURCE_FILES})

endfunction()