aux_source_directory(. API_LIB_SRCS)

add_library (AutoEngineAPI ${API_LIB_SRCS})

include_directories(${CMAKE_SOURCE_DIR}/AutoEngine/RunTime/Includes)

link_directories(${CMAKE_SOURCE_DIR}/lib)

include_directories(${PROJECT_BINARY_DIR}/config)

target_link_libraries (AutoEngineAPI  STB_IMAGE)

target_link_libraries (AutoEngineAPI  AutoEngine)

link_libraries(glfw3 opengl32 assimpd)
