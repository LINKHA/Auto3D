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


}