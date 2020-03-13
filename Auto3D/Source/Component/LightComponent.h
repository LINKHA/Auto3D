#pragma once
#include "Component/ActorComponent.h"
#include "Math/Matrix4x4.h"
#include "Math/Matrix3x4.h"
#include "Math/Color.h"

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

	void SetLightColor(const FColor& color) { _lightColor = color; }
	FColor& GetLightColor() { return _lightColor; }
public:
	/// Handle being assigned to a new parent node.
	virtual void OnActorSet(AActor* newParent, AActor* oldParent);
protected:
	TMatrix4x4F _lightView;
	TMatrix4x4F _lightProj;
	TMatrix4x4F _shadowMatrix;
	float _area = 30.0f;

	ELightType::Data _lightType;

	FColor _lightColor;
public:


	union Position
	{
		struct
		{
			float m_x;
			float m_y;
			float m_z;
			float m_w;
		};

		float m_v[4];
	};

	union LightRgbPower
	{
		struct
		{
			float m_r;
			float m_g;
			float m_b;
			float m_power;
		};

		float m_v[4];
	};

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

	void computeViewSpaceComponents(float* _viewMtx)
	{
		TVector4F viewSpaceVec4;
		if (_lightType == ELightType::DirectionalLight)
			viewSpaceVec4 = TVector4F(m_position, 0.0f);
		else
			viewSpaceVec4 = TVector4F(m_position, 1.0f);

		bx::vec4MulMtx(m_position_viewSpace, viewSpaceVec4.Data(), _viewMtx);

		float tmp[] =
		{
			  m_spotDirectionInner.m_x
			, m_spotDirectionInner.m_y
			, m_spotDirectionInner.m_z
			, 0.0f
		};
		bx::vec4MulMtx(m_spotDirectionInner_viewSpace, tmp, _viewMtx);
		m_spotDirectionInner_viewSpace[3] = m_spotDirectionInner.m_v[3];
	}

	TVector3F m_position;
	float				  m_position_viewSpace[4];
	LightRgbPower         m_ambientPower;
	LightRgbPower         m_diffusePower;
	LightRgbPower         m_specularPower;
	SpotDirectionInner    m_spotDirectionInner;
	float				  m_spotDirectionInner_viewSpace[4];
	AttenuationSpotOuter  m_attenuationSpotOuter;
};

}