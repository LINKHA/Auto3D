cmake_minimum_required(VERSION 3.1)
set(THIS_PROJECT glad)

set(_HEADERS 
glad.h
   )
	
set(_SCRS 
glad.c
)
set(_KHR
KHR/khrplatform.h
)
	
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
source_group("KHR" FILES ${_KHR})

add_library(${THIS_PROJECT} ${_SCRS} ${_HEADERS} ${_KHR})
set_target_properties(${THIS_PROJECT} PROPERTIES FOLDER "ThirdParty") 
set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx) 