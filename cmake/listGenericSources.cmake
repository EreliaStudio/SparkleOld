function(listGenericSources SOURCE_FILES_VAR)

	# --- Common to both Linux and Windows
	file(GLOB COMMON_SOURCE_FILES  
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
		"${CMAKE_SOURCE_DIR}/src/widget/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/data_structure/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/design_pattern/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/iostream/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/math/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/math/perlin/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/threading/*.cpp"
		"${CMAKE_SOURCE_DIR}/src/debug/*.cpp"
	)

	# --- Specific Windows
	if (WIN32)
		file(GLOB SYSTEM_DEPENDENT_SOURCE_FILES
			"${CMAKE_SOURCE_DIR}/src/basic_functions/windows/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/network/windows/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/input/windows/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/application/modules/windows/*.cpp"
		)

	# --- Specific Linux
	elseif(UNIX)
		file(GLOB SYSTEM_DEPENDENT_SOURCE_FILES
			"${CMAKE_SOURCE_DIR}/src/basic_functions/linux/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/network/linux/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/input/linux/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/application/modules/linux/*.cpp"
		)

	endif()
	
	set(${SOURCE_FILES_VAR} ${${SOURCE_FILES_VAR}} ${COMMON_SOURCE_FILES} ${SYSTEM_DEPENDENT_SOURCE_FILES} PARENT_SCOPE)

endfunction()