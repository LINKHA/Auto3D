#pragma once
#include "GameFramework/Actor.h"

namespace Auto3D
{

class AUTO_API APawn : public AActor
{
	DECLARE_CLASS(APawn,AActor)
public:
	APawn() {}
	~APawn() {}
};

}