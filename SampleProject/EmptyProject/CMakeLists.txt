cmake_minimum_required(VERSION 3.1)

set(THIS_PROJECT EmptyProject)

aux_source_directory(. _SRCS)

file(GLOB_RECURSE _HEADERS  *.h *.hpp)

set(SAMPLE_SRCS
	../FreeCamera.cpp
	../FreeCamera2D.cpp
	${_SRCS}
)

set(SAMPLE_HEADERS
	../FreeCamera.h
	../FreeCamera2D.h
	${_HEADERS}
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)

add_executable (${THIS_PROJECT} ${SAMPLE_SRCS} ${SAMPLE_HEADERS})

set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx)

target_link_libraries (${THIS_PROJECT}  Auto3D)

include_directories(${CMAKE_SOURCE_DIR}/AutoEngine/Source/Includes)

include_directories(${CMAKE_SOURCE_DIR}/AutoEngine/ThirdParty)