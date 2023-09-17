function (createExportOpenGLSparkleRecipe)

	include(cmake/listGenericIncludes.cmake)
	include(cmake/listOpenGLIncludes.cmake)

	message(STATUS "Creating export rules")

	install(TARGETS SparkleOpenGL
			EXPORT SparkleOpenGLTargets
			LIBRARY DESTINATION lib/SparkleOpenGL
			ARCHIVE DESTINATION lib/SparkleOpenGL
			)

	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		SparkleOpenGLConfigVersion.cmake
		VERSION ${PACKAGE_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	install(EXPORT SparkleOpenGLTargets
		FILE SparkleOpenGLTargets.cmake
		NAMESPACE SparkleOpenGL::
		DESTINATION lib/cmake/SparkleOpenGL
	)

	configure_file(cmake/SparkleOpenGLConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/SparkleOpenGLConfig.cmake" @ONLY)
	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/SparkleOpenGLConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/SparkleOpenGLConfigVersion.cmake"
		DESTINATION lib/cmake/SparkleOpenGL
	)

	# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/includes/
	# 		DESTINATION include/SparkleOpenGL
	# 		FILES_MATCHING PATTERN "*.hpp")

	listGenericIncludes(INCLUDE_LIST)
	listOpenGLIncludes(INCLUDE_LIST)

	foreach(INCLUDE_FILE ${INCLUDE_LIST})
		string(REPLACE " - " ";" CONVERTED_INCLUDE_FILE ${INCLUDE_FILE})
		list(GET CONVERTED_INCLUDE_FILE 0 INPUT)
		list(GET CONVERTED_INCLUDE_FILE 1 OUTPUT)

		if(NOT "${OUTPUT}" STREQUAL "NOT_COPIED")
			install(FILES ${INPUT}
					DESTINATION includes/SparkleOpenGL/${dir})
		endif()
	endforeach()
endfunction()