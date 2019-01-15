cmake_minimum_required(VERSION 3.1)

set(THIS_PROJECT Boost)

file(GLOB_RECURSE _HEADERS  *.h *.hpp)

file(GLOB_RECURSE _SCRS  *.c *.cpp *.cxx *.cc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${THIS_PROJECT} ${_SCRS} ${_HEADERS} )
set_target_properties(${THIS_PROJECT} PROPERTIES FOLDER "ThirdParty") 
set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx) 

target_link_libraries (${THIS_PROJECT}  Auto3D)