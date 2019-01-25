#pragma once

#define AUTO_DEBUG 1
#define AUTO_MEMORY_DEBUG 1
#define AUTO_RELEASE !AUTO_DEBUG
#define AUTO_WIN32_CONSOLE 1

#define _OPENGL_4_6_ 0		//OpenGL 4.6
#define _OPENGL_4_PLUS_ 0	//OpenGL 4.3
#define _OPENGL_3_PLUS_ 0	//OpenGL 3.3

#define _DIRECT_3D_12 1

#define AUTO_OPENGL _OPENGL_4_6_ || _OPENGL_4_PLUS_ || _OPENGL_3_PLUS_

#define AUTO_DIRECT_X _DIRECT_3D_12

#define DebugCompoundShape 0
