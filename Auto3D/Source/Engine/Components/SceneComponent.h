#pragma once
#include "Scene/Node.h"

namespace Auto3D
{

class AUTO_API ASceneComponent : public ANode
{
	DECLARE_CLASS(ASceneComponent, ANode)
public:
	ASceneComponent() {}
	~ASceneComponent() {}
};

}