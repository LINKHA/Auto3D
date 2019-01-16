cmake_minimum_required (VERSION 3.1)

project (Auto3D)
#Use folders
set_property(GLOBAL PROPERTY USE_FOLDERS On)

# Auto root path
set(AUTO_ROOT_PATH ${CMAKE_SOURCE_DIR})

if(WIN32)
	set(AUTO_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

#add_subdirectory(${AUTO_ROOT_PATH}/AutoEditor)
add_subdirectory(${AUTO_ROOT_PATH}/AutoEngine/Source)
add_subdirectory(${AUTO_ROOT_PATH}/AutoEngine/ThirdParty)
add_subdirectory(${AUTO_ROOT_PATH}/SampleProject)


