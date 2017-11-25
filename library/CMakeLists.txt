cmake_minimum_required (VERSION 3.6)

project (download C CXX)
include(ExternalProject)

ExternalProject_Add(googletest
	PREFIX	${PROJECT_SOURCE_DIR}/googletest
	GIT_REPOSITORY	https://github.com/google/googletest.git
	GIT_TAG	master
	INSTALL_COMMAND	""
)

ExternalProject_Get_Property(googletest source_dir binary_dir)

add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest googletest)

set_target_properties(libgtest PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/gtestd.lib"
	"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
	

message(STATUS "gtest_include_dirs: " ${source_dir}/googletest/include)
message(STATUS "gtest_lib_dir: " ${binary_dir}/googlemock/gtest)

set(gtest_include_dirs ${source_dir}/googletest/include PARENT_SCOPE)
set(gtest_lib_dir ${binary_dir}/googlemock/gtest PARENT_SCOPE)