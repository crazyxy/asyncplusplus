cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME asyncxx)

project(${PROJECT_NAME} C CXX)

# The version number.
set (ASYNCXX_VERSION_MAJOR 0)
set (ASYNCXX_VERSION_MINOR 9)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

option(BUILD_SHARED_LIBS "Build asyncxx as a shared library" ON)
option(USE_CXX_EXCEPTIONS "Enable C++ exception support" ON)

if (APPLE)
	option(BUILD_FRAMEWORK "Build a Mac OS X framework instead of a library" OFF)
	if (BUILD_FRAMEWORK AND NOT BUILD_SHARED_LIBS)
		message(FATAL_ERROR "Can't build a framework with static libraries")
	endif()
endif()

# Import sub directory into the project
set(LIB_DIR ${PROJECT_SOURCE_DIR}/library)
add_subdirectory(${LIB_DIR})

# Add all source and header files so IDEs can see them
file(GLOB ASYNCXX_INCLUDE ${PROJECT_SOURCE_DIR}/include/async++/*.h)
file(GLOB ASYNCXX_SRC 
		${PROJECT_SOURCE_DIR}/src/*.h 
		${PROJECT_SOURCE_DIR}/src/*.cpp
)
source_group(include FILES ${PROJECT_SOURCE_DIR}/include/async++.h ${ASYNCXX_INCLUDE})
source_group(src FILES ${ASYNCXX_SRC})

# Output of the project. Library
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/include/async++.h ${ASYNCXX_INCLUDE} ${ASYNCXX_SRC})

# asyncxx only depends on the C++11 standard libraries, but some implementations
# require the -pthread compiler flag to enable threading functionality.
if (NOT MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE -std=c++11)
endif()
if (APPLE)
	# Use libc++ on Mac because the shipped libstdc++ version is ancient
	target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS -stdlib=libc++)
endif()
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)

# Minimize the set of symbols exported by libraries
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN ON)

# Set up preprocessor definitions
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
set_target_properties(${PROJECT_NAME} PROPERTIES DEFINE_SYMBOL LIBASYNC_BUILD)
if (NOT BUILD_SHARED_LIBS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC LIBASYNC_STATIC)
endif()

# Enable warnings for strict C++ standard conformance
if (NOT MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Async++ doesn't make use of RTTI information, so don't generate it
if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /GR-)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -fno-rtti)
endif()

# Allow disabling exceptions, but warn the user about the consequences
if (NOT USE_CXX_EXCEPTIONS)
	message(WARNING "Exceptions have been disabled. Any operation that would "
	        "throw an exception will result in a call to std::abort() instead.")
	target_compile_definitions(${PROJECT_NAME} PUBLIC LIBASYNC_NO_EXCEPTIONS)
	if (MSVC)
		target_compile_options(${PROJECT_NAME} PUBLIC /EHs-c-)
	else()
		target_compile_options(${PROJECT_NAME} PUBLIC -fno-exceptions)
	endif()
endif()

#Test
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include/async++)
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME} Async++ libgtest libgmock)
#target_include_directories(${PROJECT_TEST_NAME} ${PROJECT_SOURCE_DIR}/include)
add_test(test ${PROJECT_TEST_NAME})

# Install the library and produce a CMake export script
install(TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	FRAMEWORK DESTINATION Frameworks
)
export(EXPORT ${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME} DESTINATION cmake)
if (APPLE AND BUILD_FRAMEWORK)
	set_target_properties(${PROJECT_NAME}PROPERTIES OUTPUT_NAME Async++ FRAMEWORK ON)
	set_source_files_properties(${ASYNCXX_INCLUDE} PROPERTIES MACOSX_PACKAGE_LOCATION Headers/async++)
	set_source_files_properties(${PROJECT_SOURCE_DIR}/include/async++.h PROPERTIES MACOSX_PACKAGE_LOCATION Headers)
else()
	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME async++)
	target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
	install(FILES ${PROJECT_SOURCE_DIR}/include/async++.h DESTINATION include)
	install(FILES ${ASYNCXX_INCLUDE} DESTINATION include/async++)
endif()

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "none") #required

INCLUDE(CPack)
