#pragma once
#include "Component/ActorComponent.h"

namespace Auto3D
{

class AUTO_API ALightComponent : public AActorComponent
{
	DECLARE_A_CLASS(ALightComponent, AActorComponent)
public:
	ALightComponent() {}
	~ALightComponent() {}

};

}