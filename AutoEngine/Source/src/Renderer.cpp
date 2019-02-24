#include "Renderer.h"
#include "Texture.h"

#include "NewDef.h"

namespace Auto3D {

Renderer::Renderer(SharedPtr<Ambient> ambient)
	: Super(ambient)
{

}

Renderer::~Renderer()
{

}

void Renderer::SetupShadowMaps(size_t num, int size, ImageFormat format)
{
	if (size < 1)
		size = 1;
	size = NextPowerOfTwo(size);

	_shadowMaps.clear();
	for (int i = 0; i < num; i++)
		_shadowMaps.push_back(MakeShared<ShadowMap>(_ambient));

	for (auto it = _shadowMaps.begin(); it != _shadowMaps.end(); ++it)
	{
		if ((*it)->_texture->Define(TextureType::TwoD, ResourceUsage::RenderTarget, Vector2I(size, size), format, 1))
		{
			// Setup shadow map sampling with hardware depth compare
			(*it)->_texture->DefineSampler(COMPARE_BILINEAR, ADDRESS_CLAMP, ADDRESS_CLAMP, ADDRESS_CLAMP, 1);
		}
	}
}

}