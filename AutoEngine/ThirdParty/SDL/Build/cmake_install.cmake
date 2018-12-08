# Install script for directory: D:/SoundCode/sourceTest/SDL2-2.0.9

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/SDL2")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/Debug/SDL2-staticd.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/Release/SDL2-static.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/MinSizeRel/SDL2-static.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/RelWithDebInfo/SDL2-static.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/Debug/SDL2d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/Release/SDL2.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/MinSizeRel/SDL2.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/RelWithDebInfo/SDL2.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/Debug/SDL2d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/Release/SDL2.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/MinSizeRel/SDL2.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/RelWithDebInfo/SDL2.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/Debug/SDL2maind.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/Release/SDL2main.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/MinSizeRel/SDL2main.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/RelWithDebInfo/SDL2main.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2Targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2Targets.cmake"
         "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/CMakeFiles/Export/cmake/SDL2Targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2Targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/SDL2Targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/CMakeFiles/Export/cmake/SDL2Targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/CMakeFiles/Export/cmake/SDL2Targets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/CMakeFiles/Export/cmake/SDL2Targets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/CMakeFiles/Export/cmake/SDL2Targets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/CMakeFiles/Export/cmake/SDL2Targets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES
    "D:/SoundCode/sourceTest/SDL2-2.0.9/SDL2Config.cmake"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/SDL2ConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_assert.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_atomic.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_audio.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_bits.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_blendmode.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_clipboard.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_android.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_iphoneos.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_macosx.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_minimal.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_os2.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_pandora.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_psp.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_windows.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_winrt.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_config_wiz.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_copying.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_cpuinfo.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_egl.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_endian.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_error.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_events.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_filesystem.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_gamecontroller.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_gesture.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_haptic.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_hints.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_joystick.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_keyboard.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_keycode.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_loadso.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_log.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_main.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_messagebox.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_mouse.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_mutex.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_name.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_opengl.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_opengl_glext.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_opengles.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_opengles2.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_opengles2_gl2.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_opengles2_gl2ext.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_opengles2_gl2platform.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_opengles2_khrplatform.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_pixels.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_platform.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_power.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_quit.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_rect.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_render.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_revision.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_rwops.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_scancode.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_sensor.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_shape.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_stdinc.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_surface.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_system.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_syswm.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_assert.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_common.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_compare.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_crc32.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_font.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_fuzzer.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_harness.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_images.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_log.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_md5.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_memory.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_test_random.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_thread.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_timer.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_touch.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_types.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_version.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_video.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/SDL_vulkan.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/begin_code.h"
    "D:/SoundCode/sourceTest/SDL2-2.0.9/include/close_code.h"
    "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/include/SDL_config.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/Project/MyProject/Auto3D/AutoEngine/ThirdParty/SDL/Build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
