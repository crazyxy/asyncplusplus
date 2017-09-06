cmake_minimum_required (VERSION 3.1)

project (dep_lib)
include(ExternalProject)
set(GTEST_FORCE_SHARED_CRT ON)
set(GTEST_DISABLE_PTHREADS OFF)

if(MINGW)
    set(GTEST_DISABLE_PTHREADS ON)
endif()


#################################
# Download and install GoogleTest

include (ExternalProject)
ExternalProject_Add(googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest
    # Disable install step
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(googletest source_dir binary_dir)

if(debug_mode)
	message(STATUS "source_dir: " ${source_dir})
	message(STATUS "binary_dir: " ${binary_dir})
endif()

add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest googletest)

add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock googletest)

if(WIN32)
	set_target_properties(libgtest PROPERTIES
		"IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/gtest.lib"
		"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
	)
	set_target_properties(libgmock PROPERTIES
		"IMPORTED_LOCATION" "${binary_dir}/googlemock/gmock.lib"
		"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
	)
else()
	set_target_properties(libgtest PROPERTIES
		"IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
		"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
	)
	set_target_properties(libgmock PROPERTIES
		"IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
		"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
	)
endif()

set(gtest_include_dirs ${source_dir}/googletest/include PARENT_SCOPE)
set(gtest_lib_dir ${binary_dir}/googlemock/gtest PARENT_SCOPE)
