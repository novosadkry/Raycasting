cmake_minimum_required(VERSION 3.10)

project(Raycasting)

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
target_include_directories(${TARGET} PRIVATE "libs/SFML/include")