#include "Component/SkyboxComponent.h"
#include "Gameplay/Actor.h"
#include "Gameplay/World.h"

namespace Auto3D
{
FShaderProgram ASkyboxComponent::s_hdrProgram;
FShaderProgram ASkyboxComponent::s_iblProgram;

ASkyboxComponent::ASkyboxComponent() :
	_skyboxType(ESkyboxType::HDR),
	_iblTexture(nullptr),
	_iblTextureIrrance(nullptr),
	_hdrTexture(nullptr)
{
	s_hdrProgram.AttachShader("vs_hdr_skybox", "fs_hdr_skybox");
	s_iblProgram.AttachShader("vs_ibl_skybox", "fs_ibl_skybox");
	_program = s_hdrProgram;
}

ASkyboxComponent::~ASkyboxComponent()
{

}

void ASkyboxComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ASkyboxComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

void ASkyboxComponent::SetSkyboxType(ESkyboxType::Data type)
{
	if (_skyboxType = type)
		return;

	_skyboxType = type;

	switch (_skyboxType)
	{
	case Auto3D::ESkyboxType::IBL:
		_program = s_iblProgram;
		break;
	case Auto3D::ESkyboxType::HDR:
		_program = s_hdrProgram;
		break;
	default:
		break;
	}

}
void ASkyboxComponent::SetIBLTexture(OTexture* texture, OTexture* irranceTexture)
{
	if (texture && irranceTexture)
	{
		SetSkyboxType(ESkyboxType::IBL);
		_iblTexture = texture;
		_iblTextureIrrance = irranceTexture;
	}
		
}

OTexture* ASkyboxComponent::GetIBLTexture()
{
	return _iblTexture;
}


OTexture* ASkyboxComponent::GetIBLIrranceTexture()
{
	return _iblTextureIrrance;
}

void ASkyboxComponent::SetHdrTexture(OTexture* texture)
{
	if (texture)
	{
		SetSkyboxType(ESkyboxType::HDR);
		_hdrTexture = texture;
	}
}

OTexture* ASkyboxComponent::GetHDRTexture()
{
	return _hdrTexture;
}

}