#pragma once
#include "AutoConfig.h"
#include "Component/ActorComponent.h"
#include "Particle/ParticleDef.h"

namespace Auto3D
{

struct FEmitter
{

	void Create();

	void Destroy();

	void Update();

	void imgui();

	void gizmo(const float* view, const float* proj);

	FEmitterUniforms _uniforms;
	FEmitterHandle   _handle;

	EEmitterShape::Data _shape;
	EEmitterDirection::Data _direction;

};

class AUTO_API AParticleComponent : public AActorComponent
{
	DECLARE_A_CLASS(AParticleComponent, AActorComponent)
public:
	AParticleComponent();
	~AParticleComponent();

	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;

private:
	/// Particle emitter,There's only one emitter per particle component.
	FEmitter _emitter;
};

}