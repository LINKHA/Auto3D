cmake_minimum_required (VERSION 2.8)

project (Auto)

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

add_subdirectory(./AutoEngine)
add_subdirectory(./AutoEngineAPI)

include_directories(${CMAKE_SOURCE_DIR}/AutoEngine/RunTime/Includes)
include_directories(${CMAKE_SOURCE_DIR}/AutoEngineAPI)

link_directories(${CMAKE_SOURCE_DIR}/lib)
link_libraries(glfw3 opengl32 assimpd)

add_library(STB_IMAGE "${CMAKE_SOURCE_DIR}/lib/CppLib/stb_image.cpp")
add_library(GLAD "${CMAKE_SOURCE_DIR}/lib/CppLib/glad.c")

configure_file (
  "${PROJECT_SOURCE_DIR}/CMake/Config/config.h.in"
  "${PROJECT_BINARY_DIR}/config/config.h"
)

SET(HEADERS
  EmptyProject/Launch.h
)

#file(GLOB_RECURSE _HEADERS  *.h *.hpp)

aux_source_directory(./EmptyProject DIR_SRCS)

add_executable (EmptyProject ${DIR_SRCS} ${HEADERS})

target_link_libraries (EmptyProject  STB_IMAGE)
target_link_libraries (EmptyProject  GLAD)
target_link_libraries (EmptyProject  AutoEngine)
target_link_libraries (EmptyProject  AutoEngineAPI)
