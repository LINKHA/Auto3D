#include "TextureMap.h"
AUTO_BEGIN

TextureMap::TextureMap()
{}
TextureMap::TextureMap(Ambient* ambient)
	:Super(ambient)
{}
TextureMap::~TextureMap()
{}


void TextureMap::Start()
{}

void TextureMap::Draw()
{}
void TextureMap::SetTexParameters(const TexParams & params)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
}


AUTO_END