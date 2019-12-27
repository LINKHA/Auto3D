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

	/// Add child node and add to node.
	void AddComponent(AComponent* component);

	/// Add component and create child node of specified type. A registered object factory for the type is required.
	AComponent* CreateComponent(FStringHash childType);
	/// Add component and create named child node of specified type.
	AComponent* CreateComponent(FStringHash childType, const FString& childName);
	/// Add component and create named child node of specified type.
	AComponent* CreateComponent(FStringHash childType, const char* childName);

	/// Add component and create child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateComponent() { return static_cast<_Ty*>(CreateComponent(_Ty::GetTypeHashStatic())); }
	/// Add component and create named child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateComponent(const FString& childName) { return static_cast<_Ty*>(CreateComponent(_Ty::GetTypeHashStatic(), childName)); }
	/// Add component and create named child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateComponent(const char* childName) { return static_cast<_Ty*>(CreateComponent(_Ty::GetTypeHashStatic(), childName)); }


	AComponent* GetComponent();

	AComponent* CreateComponent();

	void RemoveComponent();

	void FindComponent();

private:

	TVector<AActor*> _children;

	THashSet<AComponent*> _ownedComponents;
};
}