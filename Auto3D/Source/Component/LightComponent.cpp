#include "Component/LightComponent.h"
#include "Gameplay/Actor.h"
#include "Component/TransformComponent.h"

#include "Resource/Material.h"
#include "Resource/ResourceCache.h"

#include <bx/math.h>
#include <bgfx/bgfx.h>

namespace Auto3D
{

FShadowMap::FShadowMap():
	_shadowMapFrameBuffer(BGFX_INVALID_HANDLE),
	_fbtexture(BGFX_INVALID_HANDLE),
	_size(512)
{}

void FShadowMap::SetSize(int size)
{
	_size = size;
}

void FShadowMap::CreateTexture()
{
	_fbtexture = bgfx::createTexture2D(
		_size
		, _size
		, false
		, 1
		, bgfx::TextureFormat::D16
		, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
	);

	_shadowMapFrameBuffer = bgfx::createFrameBuffer(1, &_fbtexture, true);
}

ALightComponent::ALightComponent()
{

}

ALightComponent::~ALightComponent() 
{

}

void ALightComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ALightComponent::TickComponent(float deltaTime)
{
	Super::BeginPlay();
	//Temp
	TMatrix4x4F viewMatrix = EffectiveWorldTransform().Inverse().ToMatrix4().Transpose();
	_lightView = viewMatrix;

	float lightProj[16];
	bx::mtxOrtho(lightProj, -_area, _area, -_area, _area, -100.0f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
	_lightProj = TMatrix4x4F(lightProj);

	const bgfx::Caps* caps = bgfx::getCaps();

	const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
	const float sz = caps->homogeneousDepth ? 0.5f : 1.0f;
	const float tz = caps->homogeneousDepth ? 0.5f : 0.0f;

	TMatrix4x4F mtxCrop({
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f,   sy, 0.0f, 0.0f,
		0.0f, 0.0f, sz,   0.0f,
		0.5f, 0.5f, tz,   1.0f,
		});

	TMatrix4x4F tempMatrix;
	tempMatrix = _lightProj * mtxCrop;

	_shadowMatrix = _lightView * tempMatrix;
}

TMatrix3x4F ALightComponent::EffectiveWorldTransform() const
{
	TMatrix3x4F worldTransform(GetOwner()->GetTransform()->GetWorldPosition(), GetOwner()->GetTransform()->GetWorldRotation(), 1.0f);
	return worldTransform;
}

void ALightComponent::SetupShadowMap(int size)
{
	size = NextPowerOfTwo(size);

	_shadowMap.SetSize(size);
	_shadowMap.CreateTexture();
}

}