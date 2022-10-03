cmake_minimum_required(VERSION 3.16)

project(cookiec VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(
	${PROJECT_SOURCE_DIR}/src
	SOURCES
)

add_executable(
	cookiec
	${SOURCES}
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
	add_definitions(-DDEBUG)
	add_compile_options(-g)
	message(STATUS "Debug Mode")
else ()
	add_definitions(-DRELEASE)
	message(STATUS "Release Mode")
endif ()

target_include_directories(
		cookiec
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/inc
)
