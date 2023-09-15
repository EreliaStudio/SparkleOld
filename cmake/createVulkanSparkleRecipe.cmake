function(createVulkanRecipe)
	message(STATUS "Creating Vulkan recipe")
	set(ProductName "Sparkle")

	include(cmake/listGenericSources.cmake)
	include(cmake/listVulkanSources.cmake)
	
	find_package(Vulkan REQUIRED)
	include_directories(${Vulkan_INCLUDE_DIRS})
	if (UNIX)
	add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
	elseif(WIN32)
	add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
	endif()
	link_libraries(${Vulkan_LIBRARIES})

	listGenericSources(SOURCE_FILES)
	listVulkanSources(SOURCE_FILES)

	add_library(${ProductName} STATIC ${SOURCE_FILES})

	if (UNIX)
		target_link_libraries(${ProductName} xcb)
	endif()
	
	target_include_directories(${ProductName}
		PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
		INTERFACE $<INSTALL_INTERFACE:include>
	)
	
endfunction()