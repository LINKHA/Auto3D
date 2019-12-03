cmake_minimum_required(VERSION 3.1)

set (TARGET_NAME Auto3D)

# Generate the AutoConfig.h file
configure_file (AutoConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/AutoConfig.h)


file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(ALL_FILES ${project_headers} ${project_cpps})
source_group_by_dir(all_files)

include_directories (${AUTO_ROOT_PATH}/Auto3D/Source)

# Define the Auto3D library target
add_library (${TARGET_NAME} ${AUTO_LIB_TYPE} ${ALL_FILES})

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
