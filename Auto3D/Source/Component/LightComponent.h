#pragma once
#include "Component/ActorComponent.h"
#include "Math/Matrix4x4.h"
#include "Math/Matrix3x4.h"
#include <bgfx/bgfx.h>
namespace Auto3D
{

struct FShadowMap
{
	FShadowMap();
	void SetSize(int size);
	void CreateTexture();

	bgfx::TextureHandle _fbtexture;
	bgfx::FrameBufferHandle _shadowMapFrameBuffer;
	uint16_t _size;
};

class AUTO_API ALightComponent : public AActorComponent
{
	DECLARE_A_CLASS(ALightComponent, AActorComponent)
public:
	ALightComponent();
	~ALightComponent();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

	TMatrix3x4F EffectiveWorldTransform() const;

	TMatrix4x4F& GetLightView() { return _lightView; }
	TMatrix4x4F& GetLightProj() { return _lightProj; }
	TMatrix4x4F& GetMtxShadow() { return _shadowMatrix; }
private:
	TMatrix4x4F _lightView;
	TMatrix4x4F _lightProj;
	TMatrix4x4F _shadowMatrix;

	float _area = 30.0f;
};

}