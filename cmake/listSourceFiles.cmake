function(listSourceFiles SOURCE_FILES_VAR SYSTEM_FILES_VAR)
	file(GLOB SOURCE_FILES 
		"${CMAKE_SOURCE_DIR}/src/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/basic_functions/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/input/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/system/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/network/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/miscellaneous/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/miscellaneous/JSON/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/application/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/application/modules/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/graphics/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/widget/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/data_structure/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/design_pattern/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/iostream/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/math/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/threading/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/debug/*.cpp"
	)

	if(UNIX)
		file(GLOB SYSTEM_DEPENDENT_SOURCE_FILES
			"${CMAKE_SOURCE_DIR}/src/basic_functions/linux/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/network/linux/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/graphics/linux/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/input/linux/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/application/modules/linux/*.cpp"
		)

	elseif(WIN32)
		file(GLOB SYSTEM_DEPENDENT_SOURCE_FILES
			"${CMAKE_SOURCE_DIR}/src/basic_functions/windows/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/network/windows/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/graphics/windows/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/input/windows/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/application/modules/windows/*.cpp"
		)
	endif()
	
	set(${SOURCE_FILES_VAR} ${SOURCE_FILES} PARENT_SCOPE)
	set(${SYSTEM_FILES_VAR} ${SYSTEM_DEPENDENT_SOURCE_FILES} PARENT_SCOPE)
endfunction()

function(setupLibraryInstallation)
	install(TARGETS ${PROJECT_NAME}
			EXPORT ${PROJECT_NAME}Targets
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib
			RUNTIME DESTINATION bin
			INCLUDES DESTINATION includeTest
			)

	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		${PROJECT_NAME}ConfigVersion.cmake
		VERSION ${PACKAGE_VERSION}
		COMPATIBILITY AnyNewerVersion
	)

	install(EXPORT ${PROJECT_NAME}Targets
		FILE ${PROJECT_NAME}Targets.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION lib/cmake/${PROJECT_NAME}
	)

	configure_file(cmake/${PROJECT_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		DESTINATION lib/cmake/${PROJECT_NAME}
	)

	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/includes/
			DESTINATION include
			FILES_MATCHING PATTERN "*.hpp")
endfunction()