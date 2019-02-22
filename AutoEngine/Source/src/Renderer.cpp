#include "Renderer.h"

#include "NewDef.h"

namespace Auto3D {

Renderer::Renderer(SharedPtr<Ambient> ambient)
	: Super(ambient)
{

}

Renderer::~Renderer()
{

}
/*
void Renderer::SetupShadowMaps(size_t num, int size, ImageFormat format)
{
	if (size < 1)
		size = 1;
	size = NextPowerOfTwo(size);

	shadowMaps.Resize(num);
	for (auto it = shadowMaps.Begin(); it != shadowMaps.End(); ++it)
	{
		if (it->texture->Define(TEX_2D, USAGE_RENDERTARGET, IntVector2(size, size), format, 1))
		{
			// Setup shadow map sampling with hardware depth compare
			it->texture->DefineSampler(COMPARE_BILINEAR, ADDRESS_CLAMP, ADDRESS_CLAMP, ADDRESS_CLAMP, 1);
		}
	}
}*/
}