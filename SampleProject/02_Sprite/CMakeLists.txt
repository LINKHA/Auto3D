cmake_minimum_required(VERSION 3.1)

set (TARGET_NAME 02_Sprite)

file (GLOB SOURCE_FILES *.cpp *.h)

set(HANDER_DIR ${AUTO_ROOT_PATH}/SampleProject/Sample.h)
			  
Set(INLINE_DIR ${AUTO_ROOT_PATH}/SampleProject/Sample.inl)

add_executable (${TARGET_NAME} ${SOURCE_FILES} ${HANDER_DIR} ${INLINE_DIR})

set_target_properties(${THIS_PROJECT} PROPERTIES FOLDER "Sample") 

set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx)

target_link_libraries (${TARGET_NAME} Auto3D)