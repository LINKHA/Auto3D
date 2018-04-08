#include "GLStateCache.h"


static UInt32 beforeVerIndex = 0;  
static GLuint s_VAO = 0;
static GLuint s_VBO = 0;
static GLuint s_EBO = 0;



void enableVertexAttribs(UInt32 verIndex)
{
	for (int i = 0; i < VERTEX_SIZE; i++)
	{
		unsigned int bit = 1 << i;
		bool enabled = (verIndex & bit) != 0;
		bool enabledBefore = (beforeVerIndex & bit) != 0;
		if (enabled != enabledBefore)
		{
			if (enabled)
				glEnableVertexAttribArray(i);
			else
				glDisableVertexAttribArray(i);
		}
	}
	beforeVerIndex = verIndex;
}
void text()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}