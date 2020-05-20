#pragma once
#include "Component/ActorComponent.h"
#include "Renderer/ShaderProgram.h"
#include "Resource/Texture.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{
namespace ESkyboxTextureType
{
	enum Data
	{

	};
}

struct FSkyboxTexture
{

};


namespace ESkyboxType
{
	enum Data
	{
		IBL,
		HDR
	};
}

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

	void SetSkyboxType(ESkyboxType::Data type);
	ESkyboxType::Data GetSkyboxType() { return _skyboxType; }

	void SetIBLTexture(OTexture* texture, OTexture* irranceTexture);

	OTexture* GetIBLTexture();
	OTexture* GetIBLIrranceTexture();

	void SetHdrTexture(OTexture* texture);
	OTexture* GetHDRTexture();

	FShaderProgram& GetShaderProgram() { return _program; }

	static FShaderProgram s_hdrProgram;
	static FShaderProgram s_iblProgram;
private:
	ESkyboxType::Data _skyboxType;

	FShaderProgram _program;

	FSkyboxTexture _skyboxTexture;

	OTexture* _iblTexture;

	OTexture* _iblTextureIrrance;

	OTexture* _hdrTexture;
};

}