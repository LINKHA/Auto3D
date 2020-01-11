function( add_bgfx_shader FILE FOLDER )
	get_filename_component( FILENAME "${FILE}" NAME_WE )
	string( SUBSTRING "${FILENAME}" 0 2 TYPE )
	if( "${TYPE}" STREQUAL "fs" )
		set( TYPE "FRAGMENT" )
		set( D3D_PREFIX "ps" )
	elseif( "${TYPE}" STREQUAL "vs" )
		set( TYPE "VERTEX" )
		set( D3D_PREFIX "vs" )
	elseif( "${TYPE}" STREQUAL "cs" )
		set( TYPE "COMPUTE" )
 		set( D3D_PREFIX "cs" )
	else()
		set( TYPE "" )
	endif()

	if( NOT "${TYPE}" STREQUAL "" )
		set( COMMON FILE ${FILE} ${TYPE} INCLUDES ${BGFX_DIR}/src )
		set( OUTPUTS "" )
		set( OUTPUTS_PRETTY "" )

		if( WIN32 )
			# dx9
			if( NOT "${TYPE}" STREQUAL "COMPUTE" )
				set( DX9_OUTPUT ${AUTO_ROOT_PATH}/Auto3D/Runtime/shaders/dx9/${FILENAME}.bin )
				shaderc_parse( DX9 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_3_0 O 3 OUTPUT ${DX9_OUTPUT} )
				list( APPEND OUTPUTS "DX9" )
				set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX9, " )
			endif()

			# dx11
			set( DX11_OUTPUT ${AUTO_ROOT_PATH}/Auto3D/Runtime/shaders/dx11/${FILENAME}.bin )
			if( NOT "${TYPE}" STREQUAL "COMPUTE" )
				shaderc_parse( DX11 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_5_0 O 3 OUTPUT ${DX11_OUTPUT} )
			else()
				shaderc_parse( DX11 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_5_0 O 1 OUTPUT ${DX11_OUTPUT} )
			endif()
			list( APPEND OUTPUTS "DX11" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX11, " )
		endif()

		if( APPLE )
			# metal
			set( METAL_OUTPUT ${AUTO_ROOT_PATH}/Auto3D/Runtime/shaders/metal/${FILENAME}.bin )
			shaderc_parse( METAL ${COMMON} OSX PROFILE metal OUTPUT ${METAL_OUTPUT} )
			list( APPEND OUTPUTS "METAL" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}Metal, " )
		endif()

		# essl
		if( NOT "${TYPE}" STREQUAL "COMPUTE" )
			set( ESSL_OUTPUT ${AUTO_ROOT_PATH}/Auto3D/Runtime/shaders/essl/${FILENAME}.bin )
			shaderc_parse( ESSL ${COMMON} ANDROID OUTPUT ${ESSL_OUTPUT} )
			list( APPEND OUTPUTS "ESSL" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}ESSL, " )
		endif()

		# glsl
		set( GLSL_OUTPUT ${AUTO_ROOT_PATH}/Auto3D/Runtime/shaders/glsl/${FILENAME}.bin )
		if( NOT "${TYPE}" STREQUAL "COMPUTE" )
			shaderc_parse( GLSL ${COMMON} LINUX PROFILE 120 OUTPUT ${GLSL_OUTPUT} )
		else()
			shaderc_parse( GLSL ${COMMON} LINUX PROFILE 430 OUTPUT ${GLSL_OUTPUT} )
		endif()
		list( APPEND OUTPUTS "GLSL" )
		set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}GLSL, " )

		# spirv
		if( NOT "${TYPE}" STREQUAL "COMPUTE" )
			set( SPIRV_OUTPUT ${AUTO_ROOT_PATH}/Auto3D/Runtime/shaders/spirv/${FILENAME}.bin )
			shaderc_parse( SPIRV ${COMMON} LINUX PROFILE spirv OUTPUT ${SPIRV_OUTPUT} )
			list( APPEND OUTPUTS "SPIRV" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}SPIRV" )
			set( OUTPUT_FILES "" )
			set( COMMANDS "" )
		endif()

		foreach( OUT ${OUTPUTS} )
			list( APPEND OUTPUT_FILES ${${OUT}_OUTPUT} )
			list( APPEND COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${${OUT}} )
			get_filename_component( OUT_DIR ${${OUT}_OUTPUT} DIRECTORY )
			file( MAKE_DIRECTORY ${OUT_DIR} )
		endforeach()

		file( RELATIVE_PATH PRINT_NAME ${BGFX_DIR}/examples ${FILE} )
		add_custom_command(
			MAIN_DEPENDENCY
			${FILE}
			OUTPUT
			${OUTPUT_FILES}
			${COMMANDS}
			COMMENT "Compiling shader ${PRINT_NAME} for ${OUTPUTS_PRETTY}"
		)
	endif()
endfunction()


function( add_example ARG_NAME )
	# Parse arguments
	cmake_parse_arguments( ARG "COMMON" "" "DIRECTORIES;SOURCES" ${ARGN} )

	# Get all source files
	file(GLOB_RECURSE SOURCE_HEADER *.h)
	file(GLOB_RECURSE SOURCE_SRC *.cpp)
	file(GLOB_RECURSE SOURCE_SHADER *.sc )
	if( APPLE )
		file( GLOB SOURCE_MM *.mm )
		list( APPEND SOURCE_SRC ${SOURCE_MM} )
	endif()
		
	set(SOURCE_ALL_FILES ${SOURCE_HEADER} ${SOURCE_SRC} ${SOURCE_SHADER})
	source_group_by_dir(SOURCE_ALL_FILES)

	# Add target

	if( BGFX_INSTALL_EXAMPLES )
		add_executable( ${ARG_NAME} WIN32 ${SOURCE_ALL_FILES} )
	else()
		add_executable( ${ARG_NAME} WIN32 EXCLUDE_FROM_ALL ${SOURCE_ALL_FILES} )
	endif()
	#target_link_libraries( ${ARG_NAME} commonEng )
	target_link_libraries( ${ARG_NAME} Auto3D )
	configure_debugging( ${ARG_NAME} WORKING_DIR ${AUTO_ROOT_PATH}/Auto3D/Runtime )
	if( MSVC )
		set_target_properties( ${ARG_NAME} PROPERTIES LINK_FLAGS "/ENTRY:\"mainCRTStartup\"" )
	endif()

	if( IOS )
		set_target_properties(${ARG_NAME} PROPERTIES MACOSX_BUNDLE ON
													MACOSX_BUNDLE_GUI_IDENTIFIER ${ARG_NAME}
													MACOSX_BUNDLE_BUNDLE_VERSION 0
													MACOSX_BUNDLE_SHORT_VERSION_STRING 0
													XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
	endif()

	target_compile_definitions( ${ARG_NAME} PRIVATE "-D_CRT_SECURE_NO_WARNINGS" "-D__STDC_FORMAT_MACROS" "-DENTRY_CONFIG_IMPLEMENT_MAIN=1" )

	# Configure shaders
	if(NOT IOS AND NOT EMSCRIPTEN)
		foreach( SHADER ${SHADERS} )
			add_bgfx_shader( ${SHADER} ${ARG_NAME} )
		endforeach()
		source_group( "Shader Files" FILES ${SHADERS})
	endif()

	if (EMSCRIPTEN)
		target_link_libraries(${ARG_NAME}
			"-s PRECISE_F32=1"
			"-s TOTAL_MEMORY=268435456"
			"--memory-init-file 1")
	endif()

	# Directory name
	#set_target_properties( ${ARG_NAME} PROPERTIES FOLDER "Examples" )

	if (IOS OR WIN32)
		# on iOS we need to build a bundle so have to copy the data rather than symlink
		# and on windows we can't create symlinks
		add_custom_command( TARGET ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -E copy_directory ${AUTO_ROOT_PATH}/Auto3D/Runtime/ $<TARGET_FILE_DIR:${ARG_NAME}>)
	else()
		# For everything else symlink some folders into our output directory
		add_custom_command( TARGET ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/font $<TARGET_FILE_DIR:${ARG_NAME}>/font)
		add_custom_command( TARGET ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/images $<TARGET_FILE_DIR:${ARG_NAME}>/images)
		add_custom_command( TARGET ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/meshes $<TARGET_FILE_DIR:${ARG_NAME}>/meshes)
		add_custom_command( TARGET ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/shaders $<TARGET_FILE_DIR:${ARG_NAME}>/shaders)
		add_custom_command( TARGET ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/text $<TARGET_FILE_DIR:${ARG_NAME}>/text)
		add_custom_command( TARGET ${ARG_NAME} COMMAND ${CMAKE_COMMAND} -E create_symlink ${AUTO_ROOT_PATH}/Auto3D/Runtime/textures $<TARGET_FILE_DIR:${ARG_NAME}>/textures)
	endif()


endfunction()