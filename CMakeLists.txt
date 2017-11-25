cmake_minimum_required(VERSION 3.6)

project(asyncxx VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


option(BUILD_SHARED_LIBS "Build asyncxx as a shared library" ON)

add_subdirectory(library)

set (asyncxx_include_dir ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/asyncxx)

file(GLOB src ${PROJECT_SOURCE_DIR}/src/*.cpp)

file(GLOB headers 
	${PROJECT_SOURCE_DIR}/src/*.h
	${PROJECT_SOURCE_DIR}/include/*.h
	${PROJECT_SOURCE_DIR}/include/**/*.h
)

set(include
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/asyncxx
)

include_directories(${include})
# Output of the project. Library
add_library(${PROJECT_NAME} ${src})

#Test
add_subdirectory(test)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)

# Minimize the set of symbols exported by libraries
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN ON DEFINE_SYMBOL LIBASYNC_BUILD)
