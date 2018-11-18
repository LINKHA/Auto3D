#pragma once

#include "glad/glad.h"
// glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"

enum {
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
