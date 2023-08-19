cmake_minimum_required(VERSION 3.20)

project(benchmarks)

# Link and include directories from the top-level project
set(TOP_LIB ${CMAKE_PROJECT_NAME})

# GoogleBenchmark options
set(BENCHMARK_ENABLE_TESTING NO) # Do not build tests
# More options:
#    BENCHMARK_ENABLE_EXCEPTIONS - "Enable the use of exceptions in the benchmark library."
#    BENCHMARK_ENABLE_LTO - "Enable link time optimisation of the benchmark library."
#    BENCHMARK_USE_LIBCXX - "Build and test using libc++ as the standard library."
#    BENCHMARK_ENABLE_WERROR - "Build Release candidates with -Werror."
#    BENCHMARK_FORCE_WERROR - "Build Release candidates with -Werror regardless of compiler issues."

find_package(benchmark QUIET)
if(NOT benchmark_FOUND)
	message(STATUS "benchmark package not found. The package will be downloaded.")
	include(FetchContent)
	FetchContent_Declare(
		googlebenchmark
		GIT_REPOSITORY https://github.com/google/benchmark.git
		GIT_TAG origin/main
	)

	FetchContent_MakeAvailable(googlebenchmark)
endif()

# Recurse to find all test files
file(GLOB_RECURSE SOURCE_FILES
	*.c*
)
file(GLOB_RECURSE HEADER_FILES
	*.h*
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} ${TOP_LIB} benchmark::benchmark_main)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
