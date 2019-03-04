cmake_minimum_required(VERSION 3.1)

set (TARGET_NAME 05_PBR)

file (GLOB SOURCE_FILES *.cpp *.h)

add_executable (${TARGET_NAME} ${SOURCE_FILES})

set_target_properties(${THIS_PROJECT} PROPERTIES FOLDER "Sample") 

set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx)

target_link_libraries (${TARGET_NAME} Auto3D)