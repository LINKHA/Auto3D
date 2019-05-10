cmake_minimum_required(VERSION 3.1)

set(THIS_PROJECT LibCpuId)

add_definitions (-DVERSION="0.4.0+")

#if (HAVE_STDINT_H)
    add_definitions (-DHAVE_STDINT_H)
#endif ()

# Define generated source files
if (MSVC AND AUTO_64BIT)
    enable_language (ASM_MASM)
    # Commented out due to cmake bug
    set (ASM_FILES masm-x64.asm)
    # As a temporary workaround
    add_custom_command (OUTPUT masm-x64.obj COMMAND ${CMAKE_ASM_MASM_COMPILER} -Fo ${CMAKE_CURRENT_BINARY_DIR}/masm-x64.obj -c ${CMAKE_CURRENT_SOURCE_DIR}/masm-x64.asm DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/masm-x64.asm COMMENT "Temporary workaround for MASM")
    #set (ASM_FILES ${CMAKE_CURRENT_BINARY_DIR}/masm-x64.obj ${CMAKE_CURRENT_SOURCE_DIR}/masm-x64.asm)
endif ()



file(GLOB_RECURSE _HEADERS  *.h *.hpp)

file(GLOB_RECURSE _SCRS  *.c *.cpp *.cxx *.cc)

list (APPEND SOURCE_FILES ${ASM_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(${THIS_PROJECT} ${_SCRS} ${_HEADERS} )
set_target_properties(${THIS_PROJECT} PROPERTIES FOLDER "ThirdParty") 
set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx)