#include "tTextureMap.h"
#include "NewDef.h"

namespace Auto3D {


tTextureMap::tTextureMap(SharedPtr<Ambient> ambient)
	:Super(ambient)
{}
tTextureMap::~tTextureMap()
{}


void tTextureMap::Start()
{}

void tTextureMap::Draw()
{}
void tTextureMap::SetTexParameters(const TexParams & params)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
}


}