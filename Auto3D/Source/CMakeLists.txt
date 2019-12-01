cmake_minimum_required(VERSION 3.1)

set (TARGET_NAME Auto3D)

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

# Generate the AutoConfig.h file
configure_file (AutoConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/AutoConfig.h)

# Add source files from subdirectories
add_engine_directory (Container)
add_engine_directory (Core)
add_engine_directory_group (Core/Modules Core/Modules)
add_engine_directory (Engine)
add_engine_directory (Event)
add_engine_directory_group (Engine/Components Engine/Components)
add_engine_directory (Debug)
add_engine_directory (Graphics)
add_engine_directory (IO)
add_engine_directory (Math)
add_engine_directory (Renderer)
add_engine_directory (Resource)
add_engine_directory (Scene)
add_engine_directory (Thread)
add_engine_directory (Platform)
add_engine_directory (Audio)
add_engine_directory (Auto2D)
add_engine_directory (Physics)
add_engine_directory (Script)
add_engine_directory (Time)
add_engine_directory (Adapter)
add_engine_directory (RegisteredBox)
add_engine_directory (UI)

if (AUTO_OPENGL)
	add_engine_directory_group (Graphics/OGL Graphics/OGL)
	add_engine_directory_group (Adapter/OGL Adapter/OGL)
elseif (AUTO_VULKAN)
	add_engine_directory_group (Graphics/VK Graphics/VK)
	add_engine_directory_group (Adapter/VK Adapter/VK)
elseif (AUTO_DIRECT3D_12)
#...
endif ()

if (WIN32)
	add_engine_directory_group (Platform/Windows Platform/Windows)
else ()
	add_engine_directory_group (Platform/Android Platform/Android)
endif ()

# Add root-level files
file (GLOB AUTO_FILES *.h *.c *.cpp)
list (APPEND SOURCE_FILES ${AUTO_FILES})

include_directories (${AUTO_ROOT_PATH}/Auto3D/Source)

# Define the Auto3D library target
add_library (${TARGET_NAME} ${AUTO_LIB_TYPE} ${SOURCE_FILES})

find_package (OpenGL REQUIRED)

# Add third party include directories. Only used internally
target_link_libraries (${TARGET_NAME} ${OPENGL_gl_LIBRARY})

target_link_libraries (${TARGET_NAME}  SDL2-static)
target_link_libraries (${TARGET_NAME}  SDL2main)

include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/SDL2/include)

target_link_libraries (${TARGET_NAME}  STB)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/STB)

target_link_libraries (${TARGET_NAME}  Assimp)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/assimp/include)

target_link_libraries (${TARGET_NAME}  Box2D)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Box2D)

target_link_libraries (${TARGET_NAME}  Bullet)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Bullet)

target_link_libraries (${TARGET_NAME}  LZ4)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/LZ4)

target_link_libraries (${TARGET_NAME}  OpenAL)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/OpenAL/include)

target_link_libraries (${TARGET_NAME}  FreeType)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/FreeType/include)

target_link_libraries (${TARGET_NAME}  LibCpuId)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/LibCpuId)

target_link_libraries (${TARGET_NAME}  Imgui)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Imgui)

target_link_libraries (${TARGET_NAME}  filedialog)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/filedialog)

if (AUTO_OPENGL)
	target_link_libraries (${TARGET_NAME}  glad)
	include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/glad)
elseif (AUTO_VULKAN)
	target_link_libraries (${TARGET_NAME} vulkan-1)
	include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Vulkan)
elseif (AUTO_DIRECT3D_12)
#...
endif ()

# Add OS-specific library dependencies
if (WIN32)
    target_link_libraries (${TARGET_NAME} winmm)
else ()
    target_link_libraries (${TARGET_NAME} pthread)
endif ()
