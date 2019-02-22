#include "Texture.h"
#if AUTO_OPENGL
#include "AutoOGL.h"


namespace Auto3D {

static const unsigned glTargets[] =
{
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_3D,
	GL_TEXTURE_CUBE_MAP
};

unsigned Texture::GLTarget() const
{
	return glTargets[(int)_type];
}

}

#endif