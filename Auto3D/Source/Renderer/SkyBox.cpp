#include "SkyBox.h"
#include "../Resource/Image.h"
#include "../Graphics/Texture.h"
#include "../Debug/DebugNew.h"

namespace Auto3D
{

SkyBox::SkyBox()
{

}

SkyBox::~SkyBox()
{

}

void SkyBox::RegisterObject()
{
	RegisterFactory<SkyBox>();
}

}