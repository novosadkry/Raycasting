cmake_minimum_required(VERSION 3.10)

project(Raycasting)

macro(option_def label desc default)
	option(RAY_${label} ${desc} ${default})
	if(RAY_${label})
		add_definitions(-D${label})
	endif()
endmacro(option_def)

option_def("DEBUG" "Enables internal debugging" ON)
option_def("TRACK_MEM_ALLOC" "Enables memory tracking" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_BINARY_DIR}/bin
)

set(TARGET "RayExec")
add_executable(${TARGET})

add_subdirectory("src")
add_subdirectory("libs/SFML")

set(LIBS
	${LIBS}
	sfml-graphics
	sfml-system
	sfml-audio
)

target_link_libraries(${TARGET} ${LIBS})
target_precompile_headers(${TARGET} PRIVATE "src/Rpch.hpp")
target_include_directories(${TARGET} PRIVATE "libs/SFML/include")