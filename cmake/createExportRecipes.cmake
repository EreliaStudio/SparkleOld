function (createExportRecipes LIBRARY_NAME)
	message(STATUS "Creating export rules")

	install(TARGETS ${LIBRARY_NAME}
			EXPORT ${LIBRARY_NAME}Targets
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib
			RUNTIME DESTINATION bin
			INCLUDES DESTINATION includeTest
			)

	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		${LIBRARY_NAME}ConfigVersion.cmake
		VERSION ${PACKAGE_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	install(EXPORT ${LIBRARY_NAME}Targets
		FILE ${LIBRARY_NAME}Targets.cmake
		NAMESPACE ${LIBRARY_NAME}::
		DESTINATION lib/cmake/${LIBRARY_NAME}
	)

	configure_file(cmake/${LIBRARY_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake")
	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}ConfigVersion.cmake"
		DESTINATION lib/cmake/${LIBRARY_NAME}
	)

	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/includes/
			DESTINATION include
			FILES_MATCHING PATTERN "*.hpp")
endfunction()