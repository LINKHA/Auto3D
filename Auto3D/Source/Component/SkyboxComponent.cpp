#include "Component/SkyboxComponent.h"
#include "Gameplay/Actor.h"
#include "Gameplay/World.h"

namespace Auto3D
{

ASkyboxComponent::ASkyboxComponent() :
	_texture(nullptr),
	_textureIrrance(nullptr)
{
	_program.AttachShader("vs_ibl_skybox", "fs_ibl_skybox");
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

void ASkyboxComponent::SetTexture(OTexture* texture)
{
	if (texture)
		_texture = texture;
}

OTexture* ASkyboxComponent::GetTexture()
{
	return _texture;
}

void ASkyboxComponent::SetIrranceTexture(OTexture* texture)
{
	if (texture)
		_textureIrrance = texture;
}

OTexture* ASkyboxComponent::GetIrranceTexture()
{
	return _textureIrrance;
}

}