#pragma once
#include "Component/LightComponent.h"

#include <bx/math.h>

namespace Auto3D
{

	class AUTO_API APointLightComponent : public ALightComponent
	{
		DECLARE_A_CLASS(APointLightComponent, ALightComponent)
	public:
		APointLightComponent();
		virtual ~APointLightComponent();
		/// BeginPlay
		virtual void BeginPlay();
		/// Called every frame.
		virtual void TickComponent(float deltaTime);

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
			bx::vec4MulMtx(m_position_viewSpace, m_position.m_v, _viewMtx);

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

		Position              m_position;
		float				  m_position_viewSpace[4];
		LightRgbPower         m_ambientPower;
		LightRgbPower         m_diffusePower;
		LightRgbPower         m_specularPower;
		SpotDirectionInner    m_spotDirectionInner;
		float				  m_spotDirectionInner_viewSpace[4];
		AttenuationSpotOuter  m_attenuationSpotOuter;
	};

}