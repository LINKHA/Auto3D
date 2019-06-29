cmake_minimum_required(VERSION 3.1)

set(TARGET_NAME SpirvCompiler)

if(CMAKE_CL_64)
		link_directories(${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Vulkan/Lib/x64)
	else()
		link_directories(${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Vulkan/Lib/x86)
	endif()
	
file(GLOB_RECURSE _HEADERS  *.h *.hpp)

file(GLOB_RECURSE _SCRS  *.c *.cpp *.cxx *.cc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET_NAME} ${_SCRS} ${_HEADERS} )

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "ThirdParty") 

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE cxx)

target_link_libraries (${TARGET_NAME} vulkan-1)

include_directories (${CMAKE_SOURCE_DIR}/thire_party/Vulkan)

include_directories (${CMAKE_SOURCE_DIR}/thire_party/Vulkan/SpirvCompiler)