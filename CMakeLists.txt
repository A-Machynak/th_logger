### Directories are ignored if folder doesn't exist.

cmake_minimum_required(VERSION 3.20)

# Project name
project(th_logger)

# C++ version
set(CMAKE_CXX_STANDARD 20)

# Googletest
set(TEST_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
option(ENABLE_TEST "Build tests" ON)

# Google benchmarks
set(BENCH_DIRECTORY ${CMAKE_SOURCE_DIR}/bench)
option(ENABLE_BENCH "Build benchmarks" OFF)

# App/executable directory
set(APP_DIRECTORY ${CMAKE_SOURCE_DIR}/app)
set(APP_NAME ${CMAKE_PROJECT_NAME}_app)
option(ENABLE_APP "Build app" ON)

# Dependencies' directory
set(DEPS_DIRECTORY ${CMAKE_SOURCE_DIR}/deps)


# Disable test/bench/app if we aren't being built by this project
if(NOT ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	set(ENABLE_TEST OFF)
	set(ENABLE_BENCH OFF)
	set(ENABLE_APP OFF)
endif()

# Initialize
include(cmake/init.cmake)
