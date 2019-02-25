cmake_minimum_required(VERSION 3.1)
set(THIS_PROJECT STB)
set(_HEADERS 
stb_image.h
stb_image_write.h
stb_rect_pack.h
stb_vorbis.h
   )
	
set(_SCRS 
stb_image.cpp
)
	
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(${THIS_PROJECT} ${_SCRS} ${_HEADERS} )
set_target_properties(${THIS_PROJECT} PROPERTIES FOLDER "ThirdParty") 
set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx) 
