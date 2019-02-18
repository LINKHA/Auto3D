#include "Texture.h"

namespace Auto3D {

Texture::Texture(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, GPUObject(GetSubSystem<Graphics>())
{
}

Texture::~Texture() = default;

}