cmake_minimum_required(VERSION 3.1)

set (TARGET_NAME Auto3D)

# Generate the AutoConfig.h file
configure_file (AutoConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/AutoConfig.h)


# Get all source files
file(GLOB_RECURSE SOURCE_HEADER *.h)
file(GLOB_RECURSE SOURCE_SRC *.cpp)
file(GLOB_RECURSE SOURCE_SHADER *.sh )
if( APPLE )
	file( GLOB SOURCE_MM *.mm )
	list( APPEND SOURCE_SRC ${SOURCE_MM} )
endif()
		
set(SOURCE_ALL_FILES ${SOURCE_HEADER} ${SOURCE_SRC} ${SOURCE_SHADER})
source_group_by_dir(SOURCE_ALL_FILES)

include_directories (${AUTO_ROOT_PATH}/Auto3D/Source)

add_compile_options(-bigobj)
# Define the Auto3D library target
add_library (${TARGET_NAME} ${AUTO_LIB_TYPE} ${SOURCE_ALL_FILES})

find_package (OpenGL REQUIRED)

# Add third party include directories. Only used internally
target_link_libraries (${TARGET_NAME} ${OPENGL_gl_LIBRARY})

target_link_libraries (${TARGET_NAME}  SDL2-static)
target_link_libraries (${TARGET_NAME}  SDL2main)

include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/SDL2/include)

#target_link_libraries (${TARGET_NAME}  STB)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/bgfx/bimg/3rdparty/stb)

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

#target_link_libraries (${TARGET_NAME}  FreeType)
#include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/FreeType/include)

target_link_libraries (${TARGET_NAME}  LibCpuId)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/LibCpuId)

#target_link_libraries (${TARGET_NAME}  Imgui)

target_link_libraries (${TARGET_NAME}  dear-imgui)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/bgfx/bgfx/3rdparty/dear-imgui)

target_link_libraries (${TARGET_NAME}  filedialog)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/filedialog)

target_link_libraries (${TARGET_NAME}  rttr)
include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/rttr)

#target_link_libraries (${TARGET_NAME}  example-commonEng)
#include_directories (${AUTO_ROOT_PATH}/Auto3D/ThirdParty/bgfx/bgfx/include)

target_link_libraries(${TARGET_NAME} bgfx  )

target_link_libraries(${TARGET_NAME} meshoptimizer)

target_compile_definitions(${TARGET_NAME} PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1" )

if (IOS OR WIN32)
	# on iOS we need to build a bundle so have to copy the data rather than symlink
	# and on windows we can't create symlinks
	add_custom_command( TARGET ${TARGET_NAME} COMMAND ${CMAKE_COMMAND} -E copy_directory ${AUTO_ROOT_PATH}/Auto3D/Runtime/ $<TARGET_FILE_DIR:${TARGET_NAME}>)
else()
	# For everything else symlink some folders into our output directory
	add_custom_command( TARGET ${TARGET_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/font $<TARGET_FILE_DIR:${TARGET_NAME}>/font)
	add_custom_command( TARGET ${TARGET_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/images $<TARGET_FILE_DIR:${TARGET_NAME}>/images)
	add_custom_command( TARGET ${TARGET_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/meshes $<TARGET_FILE_DIR:${TARGET_NAME}>/meshes)
	add_custom_command( TARGET ${TARGET_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/shaders $<TARGET_FILE_DIR:${TARGET_NAME}>/shaders)
	add_custom_command( TARGET ${TARGET_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/text $<TARGET_FILE_DIR:${TARGET_NAME}>/text)
	add_custom_command( TARGET ${TARGET_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/textures $<TARGET_FILE_DIR:${TARGET_NAME}>/textures)
endif()
	

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
