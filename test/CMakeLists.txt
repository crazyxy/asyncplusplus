cmake_minimum_required (VERSION 3.1)

set(project_name "asyncxx_test")

project(${project_name} C CXX)

file(GLOB test_src ${PROJECT_SOURCE_DIR}/**/*.cpp)
add_executable(${project_name} ${test_src})
target_include_directories(${project_name} PUBLIC ${gtest_include_dirs} ${asyncxx_include_dir})
target_link_libraries(${project_name} libgtest libgmock ${asyncxx})
add_test(test ${project_name})

if(debug_mode)
	message(STATUS "project name: " ${project_name})

	foreach(file ${test_src})
		message(STATUS "test file: " ${file})
	endforeach()

	message(STATUS "include directories: ")
	message(STATUS "googletest: " ${gtest_include_dirs})
	message(STATUS "asyncxx: " ${asyncxx_include_dir})
	message(STATUS "asyncxx target: " ${asyncxx})
endif()