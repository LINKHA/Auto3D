#pragma once
#include "Container/Singleton.h"
#include "Particle/ParticleDef.h"
#include "Math/Vector3.h"

#include "AutoConfig.h"

namespace Auto3D
{
class ACameraComponent;

class AUTO_API GParticleSystem
{
	REGISTER_SINGLETON(GParticleSystem)
public:
	void Init();

	void Update(float deltaTime, ACameraComponent* camera);

	void Destory();

	//void CreateSpriteHandle();

	static FEmitterSpriteHandle s_defaultSpriteHandle;

private:

};

}