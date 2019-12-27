#pragma once
#include "Scene/Node.h"
#include "Container/HashSet.h"
#include "Engine/Components/Component.h"
#include "Scene/OctreeNode.h"

namespace Auto3D
{
class AUTO_API AActor : public AOctreeNode
{
	DECLARE_CLASS(AActor, AOctreeNode)
public:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds);

	bool AddComponent();

	AComponent* GetComponent();

	AComponent* CreateComponent();

	void RemoveComponent();

	void FindComponent();

private:

	TVector<AActor*> _children;

	THashSet<AComponent*> _ownedComponents;
};
}