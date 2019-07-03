project (Auto3D)

cmake_minimum_required (VERSION 2.8)

# Define CMake options
include (CMakeDependentOption)
# Set Auto3D as a global include directory

include_directories (Auto3D)
# Use folders
set_property(GLOBAL PROPERTY USE_FOLDERS On)

option (AUTO_SHARED "Compile the library into a health library" FALSE)
option (AUTO_LOGGING_L1 "Enable logging macros level 1" FALSE)
option (AUTO_LOGGING_L2 "Enable logging macros level 2" TRUE)
option (AUTO_PROFILING "Enable performance profiling" TRUE)
option (AUTO_OPENGL "Enable OpenGL" TRUE)
option (AUTO_DIRECT3D_12 "Enable Direct3D12" FALSE)
option (AUTO_VULKAN "Enable Vulkan" FALSE)
option (AUTO_MEMORY_DEBUG "Enable OpenGL" TRUE)
option (AUTO_WIN32_CONSOLE "Enable Direct3D12" TRUE)

# Auto root path
set(AUTO_ROOT_PATH ${CMAKE_SOURCE_DIR})

# Macro to set output directories on all configurations
macro (set_output_directories OUTPUT_PATH)
    foreach (TYPE ${ARGN})
        set (CMAKE_${TYPE}_OUTPUT_DIRECTORY ${OUTPUT_PATH})
        foreach (CONFIG ${CMAKE_CONFIGURATION_TYPES})
            string (TOUPPER ${CONFIG} CONFIG)
            set (CMAKE_${TYPE}_OUTPUT_DIRECTORY_${CONFIG} ${OUTPUT_PATH})
        endforeach ()
    endforeach ()
endmacro ()

# Set output directory for executables
set_output_directories (${PROJECT_SOURCE_DIR}/Bin RUNTIME PDB)

# Set vulkan lib
if(AUTO_VULKAN)
	if(CMAKE_CL_64)
		link_directories(${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Vulkan/Lib/x64)
	else()
		link_directories(${AUTO_ROOT_PATH}/Auto3D/ThirdParty/Vulkan/Lib/x86)
	endif()
endif()
	
add_subdirectory (Auto3D)
add_subdirectory (AutoEditor)
add_subdirectory (SampleProject)