#pragma once
#include "Component/ActorComponent.h"
#include "Math/Matrix4x4.h"
#include "Math/Matrix3x4.h"
#include "Math/Color.h"
#include "Renderer/RendererDef.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>
namespace Auto3D
{

namespace ELightType
{
	enum Data
	{
		SpotLight,
		PointLight,
		DirectionalLight,

		Count
	};
};

//namespace EShadowMapType
//{
//	enum Data
//	{
//		HARD,
//		PCF,
//		ESM,
//		VSM
//	};
//}

class AUTO_API ALightComponent : public AActorComponent
{
	DECLARE_A_CLASS(ALightComponent, AActorComponent)
public:
	ALightComponent();
	virtual ~ALightComponent();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

	TMatrix3x4F EffectiveWorldTransform() const;

	TMatrix4x4F& GetLightView() { return _lightView; }
	TMatrix4x4F& GetLightProj() { return _lightProj; }
	TMatrix4x4F& GetMtxShadow() { return _shadowMatrix; }

	ELightType::Data GetLightType() { return _lightType; }

	void SetLightColor(const FColor& color) 
	{
		m_ambientPower = m_diffusePower = m_specularPower = color;
		m_ambientPower._a = 0.02f;
		m_diffusePower._a = 0.4f;
		m_specularPower._a = 0.0f;
	}
	FColor& GetLightColor() 
	{
		return m_ambientPower;
	}

	void SetShadowType(EShadowMapImpl::Data type) { _shadowMapImpl = type; }
	EShadowMapImpl::Data GetShadowMapType() { return _shadowMapImpl; }
public:
	/// Handle being assigned to a new parent node.
	virtual void OnActorSet(AActor* newParent, AActor* oldParent);
protected:
	TMatrix4x4F _lightView;
	TMatrix4x4F _lightProj;
	TMatrix4x4F _shadowMatrix;
	float _area = 30.0f;

	ELightType::Data _lightType;
public:
	void ComputeViewSpaceComponents(float* _viewMtx)
	{
		TVector4F viewSpaceVec4;
		if (_lightType == ELightType::DirectionalLight)
			viewSpaceVec4 = TVector4F(m_position, 0.0f);
		else
			viewSpaceVec4 = TVector4F(m_position, 1.0f);

		bx::vec4MulMtx(m_position_viewSpace.Data(), viewSpaceVec4.Data(), _viewMtx);

		float tmp[] =
		{
			  m_spotDirectionInner.m_x
			, m_spotDirectionInner.m_y
			, m_spotDirectionInner.m_z
			, 0.0f
		};
		bx::vec4MulMtx(m_spotDirectionInner_viewSpace.Data(), tmp, _viewMtx);
		m_spotDirectionInner_viewSpace._w = m_spotDirectionInner.m_v[3];
	}

	union SpotDirectionInner
	{
		struct
		{
			float m_x;
			float m_y;
			float m_z;
			float m_inner;
		};

		float m_v[4];
	};

	union AttenuationSpotOuter
	{
		struct
		{
			float m_attnConst;
			float m_attnLinear;
			float m_attnQuadrantic;
			float m_outer;
		};

		float m_v[4];
	};

	TVector3F m_position;

	TVector4F m_position_viewSpace;
	// RGB for color a for power.
	FColor m_ambientPower;
	// RGB for color a for power.
	FColor m_diffusePower;
	// RGB for color a for power.
	FColor m_specularPower;

	SpotDirectionInner    m_spotDirectionInner;

	TVector4F m_spotDirectionInner_viewSpace;

	AttenuationSpotOuter  m_attenuationSpotOuter;

	EShadowMapImpl::Data _shadowMapImpl;
};

}