aux_source_directory(./RunTime/src ENGINE_LIB_SRCS)




include_directories(${CMAKE_SOURCE_DIR}/AutoEngine/RunTime/Includes)


link_directories(${CMAKE_SOURCE_DIR}/lib)

include_directories(${PROJECT_BINARY_DIR}/config)

file(GLOB_RECURSE ENGINE_HEADERS  *.h *.hpp)

#add_executable (AutoEngine ${ENGINE_HEADERS})
add_library (AutoEngine ${ENGINE_LIB_SRCS} ${ENGINE_HEADERS})

target_link_libraries (AutoEngine  STB_IMAGE)

link_libraries(glfw3 opengl32 assimpd)
