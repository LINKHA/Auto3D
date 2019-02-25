cmake_minimum_required(VERSION 3.1)
set(THIS_PROJECT LZ4)
set(_HEADERS 
lz4.h
lz4hc.h
lz4opt.h
   )
	
set(_SCRS 
lz4.c
lz4hc.c
)
	
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(${THIS_PROJECT} ${_SCRS} ${_HEADERS} )
set_target_properties(${THIS_PROJECT} PROPERTIES FOLDER "ThirdParty") 
set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx) 