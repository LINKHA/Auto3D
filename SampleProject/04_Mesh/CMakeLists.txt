cmake_minimum_required(VERSION 3.1)

set (TARGET_NAME 04_Mesh)

file (GLOB SOURCE_FILES *.cpp *.h)

set(HANDER_DIR ${AUTO_ROOT_PATH}/SampleProject/Sample.h)
			  
Set(INLINE_DIR ${AUTO_ROOT_PATH}/SampleProject/Sample.inl)

if (WIN32) 
	if (AUTO_WIN32_CONSOLE)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
	else()
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	endif()
endif()

add_executable (${TARGET_NAME} ${SOURCE_FILES} ${HANDER_DIR} ${INLINE_DIR})

target_link_libraries (${TARGET_NAME} Auto3D)
include_directories (${AUTO_ROOT_PATH}/Auto3D/Source)

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