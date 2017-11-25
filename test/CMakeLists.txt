cmake_minimum_required(VERSION 3.6)

enable_testing()

project(asyncxx_test C CXX)

file(GLOB test_src ${PROJECT_SOURCE_DIR}/**/*.cpp)

add_executable(${PROJECT_NAME} ${test_src})


foreach(file ${headers})
	message(STATUS "include directory: " ${file})
endforeach()

target_include_directories(${PROJECT_NAME} PUBLIC ${gtest_include_dirs})
target_link_libraries(${PROJECT_NAME} libgtest asyncxx)

add_test(test ${PROJECT_NAME})