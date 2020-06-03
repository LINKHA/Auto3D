#pragma once
#include "Container/Singleton.h"
#include "Particle/ParticleDef.h"

#include "AutoConfig.h"

namespace Auto3D
{

class AUTO_API GParticleSystem
{
	REGISTER_SINGLETON(GParticleSystem)
public:
	void Init();

	void Update();

	//void CreateSpriteHandle();

	static FEmitterSpriteHandle s_defaultSpriteHandle;

private:

};

}