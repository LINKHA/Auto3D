# Install script for directory: D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/Assimp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/lib/Debug/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/lib/Release/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/lib/MinSizeRel/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/lib/RelWithDebInfo/assimp-vc140-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/bin/Debug/assimp-vc140-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/bin/Release/assimp-vc140-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/bin/MinSizeRel/assimp-vc140-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/bin/RelWithDebInfo/assimp-vc140-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/anim.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/ai_assert.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/camera.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/color4.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/color4.inl"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/config.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/defs.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Defines.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/cfileio.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/light.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/material.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/material.inl"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/matrix3x3.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/matrix3x3.inl"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/matrix4x4.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/matrix4x4.inl"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/mesh.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/pbrmaterial.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/postprocess.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/quaternion.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/quaternion.inl"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/scene.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/metadata.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/texture.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/types.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/vector2.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/vector2.inl"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/vector3.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/vector3.inl"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/version.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/cimport.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/importerdesc.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Importer.hpp"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/DefaultLogger.hpp"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/ProgressHandler.hpp"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/IOStream.hpp"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/IOSystem.hpp"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Logger.hpp"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/LogStream.hpp"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/NullLogger.hpp"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/cexport.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Exporter.hpp"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/DefaultIOStream.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/DefaultIOSystem.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/SceneCombiner.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/fast_atof.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/qnan.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/BaseImporter.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Hash.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/MemoryIOWrapper.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/ParsingUtils.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/StreamReader.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/StreamWriter.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/StringComparison.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/StringUtils.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/SGSpatialSort.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/GenericProperty.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/SpatialSort.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/SmoothingGroups.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/SmoothingGroups.inl"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/StandardShapes.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/RemoveComments.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Subdivision.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Vertex.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/LineSplitter.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/TinyFormatter.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Profiler.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/LogAux.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Bitmap.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/XMLTools.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/IOStreamBuffer.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/CreateAnimMesh.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/irrXMLWrapper.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/BlobIOSystem.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/MathFunctions.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Macros.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Exceptional.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Compiler/pushpack1.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Compiler/poppack1.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/Debug/assimp-vc140-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/Assimp/code/RelWithDebInfo/assimp-vc140-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

