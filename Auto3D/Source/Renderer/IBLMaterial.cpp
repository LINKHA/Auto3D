#include "IBLMaterial.h"

namespace Auto3D
{

IBLMaterial::IBLMaterial()
{

}

IBLMaterial::~IBLMaterial()
{

}

void IBLMaterial::RegisterObject()
{
	RegisterFactory<IBLMaterial>();
}

void IBLMaterial::CreatePass()
{
	//Pass* newPass = CreatePass(it->_first);
	
	//Texture* textureCube = new Texture();
	//textureCube->Define(TextureType::TEX_CUBE, ResourceUsage::DEFAULT, imageData[0]->GetLevel(0)._size, imageData[0]->GetFormat(), 1, &faces[0]);
	//textureCube->DefineSampler(TextureFilterMode::COMPARE_TRILINEAR, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP, TextureAddressMode::CLAMP);
	//textureCube->SetDataLost(false);
	//SetTexture(jsonTextures.Begin()->_first.ToInt(), textureCube);
}

}