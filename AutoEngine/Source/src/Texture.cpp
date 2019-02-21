#include "Texture.h"

namespace Auto3D {

Texture::Texture(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, GPUObject(GetSubSystem<Graphics>())
	, _texture(0)
	, _type(TextureType::TwoD)
	, _usage(ResourceUsage::Default)
	, _size(Vector2::ZERO)
	, _format(ImageFormat::NONE)
{
}

Texture::~Texture() = default;

}