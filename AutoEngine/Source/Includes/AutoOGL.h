#pragma once

#include "glad/glad.h"
// glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Auto3D {

#ifndef COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#	define COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8c00
#endif
#ifndef COMPRESSED_RGB_PVRTC_2BPPV1_IMG
#	define COMPRESSED_RGB_PVRTC_2BPPV1_IMG 0x8c01
#endif
#ifndef COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#	define COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8c02
#endif
#ifndef COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
#	define COMPRESSED_RGBA_PVRTC_2BPPV1_IMG 0x8c03
#endif

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#	define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#	define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#	define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#	define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
enum VertexAttrib
{
	VERTEX_ATTRIB_NONE = 0,
	VERTEX_ATTRIB_POSITION = 1 << 0,
	VERTEX_ATTRIB_COLOR = 1 << 1,
	VERTEX_ATTRIB_TEXCOORD = 1 << 2,
	VERTEX_ATTRIB_NORMAL = 1 << 3,
	VERTEX_ATTRIB_BLEND_WEIGHT = 1 << 4,
	VERTEX_ATTRIB_BLEND_INDEX = 1 << 5,
	VERTEX_ATTRIB_POS_COLOR_TEX = (VERTEX_ATTRIB_POSITION | VERTEX_ATTRIB_COLOR | VERTEX_ATTRIB_TEXCOORD),
	VERTEX_SIZE
};

static void EnableVertexAttribs(unsigned int verIndex)
{
	for (int i = 0; i < VERTEX_SIZE; i++)
	{
		unsigned int bit = 1 << i;
		bool enabled = (verIndex & bit) != 0;
		if (enabled)
			glEnableVertexAttribArray(i);
		else
			glDisableVertexAttribArray(i);

	}
}

}