cmake_minimum_required(VERSION 3.20)

project(tests)

# Link and include directories from the top-level project
set(TOP_LIB ${CMAKE_PROJECT_NAME})

find_package(GTest QUIET)
if(NOT GTest_FOUND)
	message(STATUS "GTest package not found. The package will be downloaded.")

	# Download googletest - https://google.github.io/googletest/quickstart-cmake.html
	include(FetchContent)
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG origin/main
	)

	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
endif()

# Recurse to find all test files
file(GLOB_RECURSE SOURCE_FILES
	*.c*
)
file(GLOB_RECURSE HEADER_FILES
	*.h*
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} ${TOP_LIB} GTest::gtest GTest::gmock GTest::gtest_main)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
