cmake_minimum_required(VERSION 3.1)
set(THIS_PROJECT rttr)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCE_HEADER *.h)
file(GLOB_RECURSE SOURCE_SRC *.cpp)
set(SOURCE_ALL_FILES ${SOURCE_HEADER} ${SOURCE_SRC})
source_group_by_dir(SOURCE_ALL_FILES)


add_library(${THIS_PROJECT} ${SOURCE_ALL_FILES})
set_target_properties(${THIS_PROJECT} PROPERTIES FOLDER "ThirdParty") 
set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx) 