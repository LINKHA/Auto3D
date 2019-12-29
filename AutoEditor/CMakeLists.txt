cmake_minimum_required(VERSION 3.1)

set (TARGET_NAME AutoEditor)

# Macro to add files from a directory grouped to the solution file
macro (add_engine_directory DIRECTORY)
    file (GLOB DIR_FILES ${DIRECTORY}/*.h ${DIRECTORY}/*.c ${DIRECTORY}/*.cpp)
    get_filename_component (GROUP ${DIRECTORY} NAME)
    source_group ("${GROUP}" FILES ${DIR_FILES})
    list (APPEND SOURCE_FILES ${DIR_FILES})
endmacro ()

macro (add_engine_directory_group DIRECTORY GROUP)
    file (GLOB DIR_FILES ${DIRECTORY}/*.h ${DIRECTORY}/*.c ${DIRECTORY}/*.cpp)
    source_group ("${GROUP}" FILES ${DIR_FILES})
    list (APPEND SOURCE_FILES ${DIR_FILES})
endmacro ()

# Add root-level files
file (GLOB AUTO_FILES *.h *.c *.cpp)
list (APPEND SOURCE_FILES ${AUTO_FILES})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

if (WIN32) 
	if (AUTO_WIN32_CONSOLE)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
	else()
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	endif()
endif()

add_executable (${TARGET_NAME}  ${SOURCE_FILES})


target_link_libraries (${TARGET_NAME} Auto3D)

include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/SDL2/include)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/STB)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/assimp/include)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Box2D)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Bullet)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/LZ4)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/OpenAL/include)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/FreeType/include)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/LibCpuId)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Imgui)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/filedialog)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/rttr)

if (AUTO_OPENGL)
	include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/glad)
elseif (AUTO_VULKAN)
	include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Vulkan)
elseif (AUTO_DIRECT3D_12)
#...
endif ()