#pragma once
#include "Scene/Node.h"
#include "Container/HashSet.h"
#include "Engine/Components/SceneComponent.h"
#include "Scene/OctreeNode.h"

namespace Auto3D
{
class AUTO_API AActor : public AOctreeNode
{
	DECLARE_CLASS(AActor, AOctreeNode)
public:
	bool AddComponent();

	ASceneComponent* GetComponent();

private:

	TVector<AActor*> _children;

	THashSet<ASceneComponent*> _ownedComponents;
};
}