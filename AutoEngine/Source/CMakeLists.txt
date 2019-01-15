cmake_minimum_required(VERSION 3.1)

configure_file (
  "${AUTO_ROOT_PATH}/AutoEngine/Source/Includes/Config.h.in"
  "${AUTO_ROOT_PATH}/AutoEngine/Source/Includes/Config.h"
)

aux_source_directory(${AUTO_ROOT_PATH}/AutoEngine/Source/src AUTO_3D_LIB_SRCS)

include_directories(${AUTO_ROOT_PATH}/AutoEngine/Source/Includes)
include_directories(${AUTO_ROOT_PATH}/AutoEngine/ThirdParty)
include_directories(${AUTO_ROOT_PATH}/AutoEngine/ThirdParty/Assimp/include)

include_directories(${CMAKE_BINARY_DIR}/AutoEngine/Source/Includes)
include_directories(${CMAKE_BINARY_DIR}/AutoEngine/ThirdParty)
include_directories(${CMAKE_BINARY_DIR}/AutoEngine/ThirdParty/Assimp)
file(GLOB_RECURSE AUTO_3D_HEADERS  *.h *.hpp)

add_library (Auto3D ${AUTO_3D_LIB_SRCS} ${AUTO_3D_HEADERS})

set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)

target_link_libraries (Auto3D  Assimp)

target_link_libraries (Auto3D  Box2D)

target_link_libraries (Auto3D  Bullet)

target_link_libraries (Auto3D  glad)

target_link_libraries (Auto3D  KhSTL)

target_link_libraries (Auto3D  LZ4)

target_link_libraries (Auto3D  OpenAL)

target_link_libraries (Auto3D  SDL2-static)

target_link_libraries (Auto3D  SDL2main)

target_link_libraries (Auto3D  STB)