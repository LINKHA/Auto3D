#pragma once
#include "Component/ActorComponent.h"
#include "Renderer/ShaderProgram.h"
#include "Resource/Texture.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

class AUTO_API ASkyboxComponent : public AActorComponent
{
	DECLARE_A_CLASS(ASkyboxComponent, AActorComponent)
public:
	ASkyboxComponent();

	~ASkyboxComponent();

	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;

	void SetTexture(OTexture* texture);
	void SetIrranceTexture(OTexture* texture);
private:
	FShaderProgram _program;

	OTexture* _texture;

	OTexture* _textureIrrance;
};

}