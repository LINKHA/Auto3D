#include "TextureCube.h"
#if AUTO_OPENGL
#include "AutoOGL.h"

namespace Auto3D {
TextureCube::TextureCube(SharedPtr<Ambient> ambient) 
	: Super(ambient)
{
	_target = GL_TEXTURE_CUBE_MAP;

	// Default to clamp mode addressing
	_addressModes[(int)TextureCoordinate::U] = TextureAddressMode::Clamp;
	_addressModes[(int)TextureCoordinate::V] = TextureAddressMode::Clamp;
	_addressModes[(int)TextureCoordinate::W] = TextureAddressMode::Clamp;
}
}

#endif