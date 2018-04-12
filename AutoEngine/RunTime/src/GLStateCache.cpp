#include "GLStateCache.h"



void enableVertexAttribs(UInt32 verIndex)
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

void bindTexture2D(GLuint textureID)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
}