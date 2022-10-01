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

target_include_directories(
		cookiec
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/inc
)
