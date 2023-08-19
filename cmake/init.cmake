# Create a directory for headers and a header file to indicate, that this folder is for headers
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME})
	file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)
	file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME})
	file(WRITE ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/core.h "#pragma once\n")
endif()

# Recurse to find all library files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/dev/src/*.c* # library
)
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/include/*.h*
)

# Create library
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Create app
if(NOT ${ENABLE_APP})
	message(STATUS "App won't be built - ENABLE_APP == OFF")
elseif(NOT EXISTS ${APP_DIRECTORY})
	message(STATUS "No executable will be created - APP_DIRECTORY \"" ${APP_DIRECTORY} "\" not found.")
else()
	file(GLOB_RECURSE APP_FILES
		${APP_DIRECTORY}/*.c*
		${APP_DIRECTORY}/*.h*
	)
	add_executable(${APP_NAME} ${APP_FILES})
	target_link_libraries(${APP_NAME} ${PROJECT_NAME})
	target_include_directories(${APP_NAME}
		PUBLIC ${CMAKE_SOURCE_DIR}/include
		PRIVATE ${CMAKE_SOURCE_DIR}/app)
endif()

# External libraries
if(EXISTS ${DEPS_DIRECTORY})
	add_subdirectory(${DEPS_DIRECTORY})
else()
	message(STATUS "Dependencies won't be added - DEPS_DIRECTORY \"" ${DEPS_DIRECTORY} "\" not found.")
endif()

# Tests
if(NOT ENABLE_TEST)
	message(STATUS "Tests won't be built - ENABLE_TEST == OFF")
elseif(NOT EXISTS ${TEST_DIRECTORY})
	message(STATUS "Tests won't be built - TEST_DIRECTORY \"" ${TEST_DIRECTORY} "\" not found.")
else()
	add_subdirectory(${TEST_DIRECTORY})
endif()

# Benchmarks
if(NOT ENABLE_BENCH)
	message(STATUS "Benchmarks won't be built - ENABLE_BENCH == OFF")
elseif(NOT EXISTS ${BENCH_DIRECTORY})
	message(STATUS "Benchmarks won't be built - BENCH_DIRECTORY \"" ${BENCH_DIRECTORY} "\" not found.")
else()
	add_subdirectory(${BENCH_DIRECTORY})
endif()
