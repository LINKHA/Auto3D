#pragma once
#include "AutoConfig.h"
#include "Component/ActorComponent.h"
#include "ParticleSystem/ParticleSystem.h"

namespace Auto3D
{

struct FEmitter
{
	FEmitterUniforms m_uniforms;
	FEmitterHandle   m_handle;

	EEmitterShape::Data m_shape;
	EEmitterDirection::Data m_direction;

	void create();

	void destroy();

	void update();

	void imgui();

	void gizmo(const float* view, const float* proj);
};

class AUTO_API AParticleComponent : public AActorComponent
{
	DECLARE_A_CLASS(AParticleComponent, AActorComponent)
public:
	AParticleComponent();
	~AParticleComponent();


};

}