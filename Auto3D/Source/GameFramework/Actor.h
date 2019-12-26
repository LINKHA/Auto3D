#pragma once
#include "Scene/Node.h"
#include "Container/HashSet.h"
#include "Engine/Components/SceneComponent.h"

namespace Auto3D
{
class AUTO_API AActor : public ANode
{
	DECLARE_CLASS(AActor,ANode)
public:
	bool AddComponent();

	ASceneComponent* GetComponent();


private:
	THashSet<ASceneComponent*> _ownedComponents;
};
}