set(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/archive/${ArchitectureSufix}/playground")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${ArchitectureSufix}/playground")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/executable/${ArchitectureSufix}/playground")

add_executable(Playground ${SOURCE_FILES})

target_include_directories(
    Playground
	PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(Playground PUBLIC cxx_std_20)
target_link_libraries(Playground PRIVATE Sparkle)