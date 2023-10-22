function (createExportVulkanSparkleRecipe)
	message(STATUS "Creating export rules")

	include(cmake/listGenericIncludes.cmake)
	include(cmake/listVulkanIncludes.cmake)

	install(TARGETS Sparkle
			EXPORT SparkleTargets
			LIBRARY DESTINATION lib/Sparkle
			ARCHIVE DESTINATION lib/Sparkle
			)

	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		SparkleConfigVersion.cmake
		VERSION ${PACKAGE_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	install(EXPORT SparkleTargets
		FILE SparkleTargets.cmake
		NAMESPACE Sparkle::
		DESTINATION lib/cmake/Sparkle
	)

	configure_file(cmake/SparkleConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/SparkleConfig.cmake" @ONLY)
	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/SparkleConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/SparkleConfigVersion.cmake"
		DESTINATION lib/cmake/Sparkle
	)

	# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/includes/
	# 		DESTINATION include/Sparkle
	# 		FILES_MATCHING PATTERN "*.hpp")

	listGenericIncludes(INCLUDE_LIST)
	listVulkanIncludes(INCLUDE_LIST)

	foreach(INCLUDE_FILE ${INCLUDE_LIST})
		string(REPLACE " - " ";" INCLUDE_FILE ${INCLUDE_FILE})
		list(GET INCLUDE_FILE 0 INPUT)
		list(GET INCLUDE_FILE 1 OUTPUT)

		if(NOT "${OUTPUT}" STREQUAL "NOT_COPIED")
			install(FILES ${INPUT}
					DESTINATION includes/Sparkle/${OUTPUT})
		endif()
	endforeach()
endfunction()