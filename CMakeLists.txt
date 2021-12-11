cmake_minimum_required(VERSION 3.10)

project(Raycasting)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

macro(option_def label desc default)
	option(RAY_${label} ${desc} ${default})
	if(RAY_${label})
		add_definitions(-D${label})
	endif()
endmacro(option_def)

# Define build options
option_def("DEBUG" "Enables internal debugging" ON)
option_def("BUILD_W_ALL" "Show all build warnings" OFF)
option_def("TRACK_MEM_ALLOC" "Enables memory tracking" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_BINARY_DIR}/bin
)

set(TARGET "RayExec")
add_executable(${TARGET})

# Compile source
add_subdirectory("src")

# Compile SFML
set(SFML_DIR "libs/SFML")
set(SFML_INCLUDE ${SFML_DIR}/include)
set(SFML_COMPONENTS sfml-graphics sfml-system sfml-audio)
add_subdirectory(${SFML_DIR})

# Find OpenGL
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

# Compile ImGui
set(IMGUI_DIR "libs/imgui")
set(IMGUI_INCLUDE
	${IMGUI_DIR}
	${IMGUI_DIR}/misc/cpp
)
file(GLOB IMGUI_SOURCE
	${IMGUI_DIR}/*.cpp
	${IMGUI_DIR}/misc/cpp/*.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCE})
target_include_directories(imgui PRIVATE ${IMGUI_INCLUDE})

# Compile ImGui-SFML
set(IMGUI_SFML_DIR "libs/imgui-sfml")
file(GLOB IMGUI_SFML_SOURCE ${IMGUI_SFML_DIR}/*.cpp)
add_library(imgui-sfml STATIC ${IMGUI_SFML_SOURCE})
target_link_libraries(imgui-sfml imgui ${SFML_COMPONENTS})
target_include_directories(imgui-sfml PRIVATE ${IMGUI_DIR})
target_include_directories(imgui-sfml PRIVATE ${IMGUI_SFML_DIR})
target_include_directories(imgui-sfml PRIVATE ${SFML_INCLUDE})

# Include entt
set(ENTT_DIR "libs/entt")
set(ENTT_INCLUDE ${ENTT_DIR}/src)

# Include cereal
set(CEREAL_DIR "libs/cereal")
set(CEREAL_INCLUDE ${CEREAL_DIR}/include)

set(LIBS
	${LIBS}
	${SFML_COMPONENTS}
	imgui
	imgui-sfml
	OpenGL::GL
)

if (RAY_BUILD_W_ALL)
	if (MSVC)
		target_compile_options(${TARGET} PRIVATE /W4 /Wall)
	else()
		target_compile_options(${TARGET} PRIVATE -Wall -Wextra -pedantic)
	endif()
endif()

target_link_libraries(${TARGET} ${LIBS})
target_precompile_headers(${TARGET} PRIVATE "src/Rpch.hpp")
target_include_directories(${TARGET} PRIVATE ${SFML_INCLUDE})
target_include_directories(${TARGET} PRIVATE ${IMGUI_INCLUDE})
target_include_directories(${TARGET} PRIVATE ${IMGUI_SFML_DIR})
target_include_directories(${TARGET} PRIVATE ${ENTT_INCLUDE})
target_include_directories(${TARGET} PRIVATE ${CEREAL_INCLUDE})