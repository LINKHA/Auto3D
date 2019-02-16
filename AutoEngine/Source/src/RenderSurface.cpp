#include "RenderSurface.h"
#include "Texture.h"

namespace Auto3D {

RenderSurface::~RenderSurface()
{

}
SharedPtr<Texture> RenderSurface::GetParentTexture() const
{
	return _parentTexture; 
}

int RenderSurface::GetMultiSample() const
{
	return _parentTexture->GetMultiSample();
}
int RenderSurface::GetWidth() const
{
	return _parentTexture->GetWidth();
}

int RenderSurface::GetHeight() const
{
	return _parentTexture->GetHeight();
}
}