cmake_minimum_required(VERSION 3.1)

set (TARGET_NAME AutoEditor)

# Macro to add files from a directory grouped to the solution file
macro (add_engine_directory DIRECTORY)
    file (GLOB DIR_FILES ${DIRECTORY}/*.h ${DIRECTORY}/*.c ${DIRECTORY}/*.cpp)
    get_filename_component (GROUP ${DIRECTORY} NAME)
    source_group ("${GROUP}" FILES ${DIR_FILES})
    list (APPEND SOURCE_FILES ${DIR_FILES})
endmacro ()

macro (add_engine_directory_group DIRECTORY GROUP)
    file (GLOB DIR_FILES ${DIRECTORY}/*.h ${DIRECTORY}/*.c ${DIRECTORY}/*.cpp)
    source_group ("${GROUP}" FILES ${DIR_FILES})
    list (APPEND SOURCE_FILES ${DIR_FILES})
endmacro ()


# Add root-level files
file (GLOB AUTO_FILES *.h *.c *.cpp)
list (APPEND SOURCE_FILES ${AUTO_FILES})

add_executable (${TARGET_NAME} ${SOURCE_FILES})

set_target_properties(${THIS_PROJECT} PROPERTIES LINKER_LANGUAGE cxx)

target_link_libraries (${TARGET_NAME} Auto3D)