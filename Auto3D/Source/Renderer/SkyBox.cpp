#include "SkyBox.h"

namespace Auto3D
{
/// Construct.
SkyBox::SkyBox()
{

}
/// Destruct.
SkyBox::~SkyBox()
{

}

/// Register factory and attributes.
void SkyBox::RegisterObject()
{
	RegisterFactory<SkyBox>();
	CopyBaseAttributes<SkyBox, GeometryNode>();
}
}