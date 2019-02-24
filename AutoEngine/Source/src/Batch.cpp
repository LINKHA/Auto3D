#include "Batch.h"
#include "Texture.h"

namespace Auto3D {


ShadowMap::ShadowMap(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
	// Construct texture but do not define its size yet
	_texture = MakeShared<Texture>(_ambient);
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::Clear()
{
	_allocator.Reset(_texture->Width(), _texture->Height(), 0, 0, false);
	_shadowViews.clear();
	_used = false;
}

void ShadowView::Clear()
{
	shadowQueue.Clear();
}

}