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
	AActor();
	~AActor();

	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds);

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
	
	/// Add child node and add to node.
	void AddComponent(AComponent* component);
	/// Remove child node. Will delete it if there are no other strong references to it.
	void RemoveComponent(AComponent* component);
	/// Remove all child nodes.
	void RemoveAllComponents();

	/// Return all components.
	const THashSet<AComponent*>& GetAllComponents() const { return _ownedComponents; }
	/// Return child nodes recursively.
	void GetAllComponents(TVector<AComponent*>& result, bool recursive = false) const;

	/// Return first component that matches name.
	AComponent* FindComponent(const FString& childName, bool recursive = false) const;
	/// Return first component that matches name.
	AComponent* FindComponent(const char* childName, bool recursive = false) const;
	/// Return first component of specified type.
	AComponent* FindComponent(FStringHash childType, bool recursive = false) const;
	/// Return first component that matches type and name.
	AComponent* FindComponent(FStringHash childType, const FString& childName, bool recursive = false) const;
	/// Return first component that matches type and name.
	AComponent* FindComponent(FStringHash childType, const char* childName, bool recursive = false) const;

	/// Find components of specified type.
	void FindComponents(TVector<AComponent*>& result, FStringHash childType, bool recursive = false) const;

	/// Return first component of specified type, template version.
	template <typename _Ty> _Ty* FindComponent(bool recursive = false) const { return static_cast<_Ty*>(FindComponent(_Ty::GetTypeHashStatic(), recursive)); }
	/// Return first component that matches type and name, template version.
	template <typename _Ty> _Ty* FindComponent(const FString& childName, bool recursive = false) const { return static_cast<_Ty*>(FindComponent(_Ty::GetTypeHashStatic(), childName, recursive)); }
	/// Return first component that matches type and name, template version.
	template <typename _Ty> _Ty* FindComponent(const char* childName, bool recursive = false) const { return static_cast<_Ty*>(FindComponent(_Ty::GetTypeHashStatic(), childName, recursive)); }
	/// Find components of specified type, template version.
	template <typename _Ty> void FindComponents(TVector<_Ty*>& result, bool recursive = false) const { return FindComponents(reinterpret_cast<TVector<AComponent*>&>(result), _Ty::GetTypeHashStatic(), recursive); }


	/// Add actor and create child node of specified type. A registered object factory for the type is required.
	AActor* CreateChild(FStringHash childType);
	/// Add actor and create named child node of specified type.
	AActor* CreateChild(FStringHash childType, const FString& childName);
	/// Add actor and create named child node of specified type.
	AActor* CreateChild(FStringHash childType, const char* childName);

	/// Add actor and create child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateChild() { return static_cast<_Ty*>(CreateChild(_Ty::GetTypeHashStatic())); }
	/// Add component and create named child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateChild(const FString& childName) { return static_cast<_Ty*>(CreateChild(_Ty::GetTypeHashStatic(), childName)); }
	/// Add component and create named child node of the specified type, template version.
	template <typename _Ty> _Ty* CreateChild(const char* childName) { return static_cast<_Ty*>(CreateChild(_Ty::GetTypeHashStatic(), childName)); }
	
	/// Add child actor and add to node.
	void AddChild(AActor* childActor);
	/// Remove child and child node. Will delete it if there are no other strong references to it.
	void RemoveChild(AActor* childActor);
	/// Remove child and child node by index.
	void RemoveChild(size_t index);
	/// Remove all child nodes.
	void RemoveAllChildren();

	/// Return all child.
	const TVector<AActor*>& GetChildren() const { return _children; }
	/// Return child recursively.
	void GetChildren(TVector<AActor*>& result, bool recursive = false) const;

	/// Return first child that matches name.
	AActor* FindChild(const FString& childName, bool recursive = false) const;
	/// Return first child that matches name.
	AActor* FindChild(const char* childName, bool recursive = false) const;
	/// Return first child of specified type.
	AActor* FindChild(FStringHash childType, bool recursive = false) const;
	/// Return first child that matches type and name.
	AActor* FindChild(FStringHash childType, const FString& childName, bool recursive = false) const;
	/// Return first child that matches type and name.
	AActor* FindChild(FStringHash childType, const char* childName, bool recursive = false) const;

	/// Find child of specified type.
	void FindChildren(TVector<AActor*>& result, FStringHash childType, bool recursive = false) const;

	/// Return first Child of specified type, template version.
	template <typename _Ty> _Ty* FindChild(bool recursive = false) const { return static_cast<_Ty*>(FindChild(_Ty::GetTypeHashStatic(), recursive)); }
	/// Return first Child that matches type and name, template version.
	template <typename _Ty> _Ty* FindChild(const FString& childName, bool recursive = false) const { return static_cast<_Ty*>(FindChild(_Ty::GetTypeHashStatic(), childName, recursive)); }
	/// Return first Child that matches type and name, template version.
	template <typename _Ty> _Ty* FindChild(const char* childName, bool recursive = false) const { return static_cast<_Ty*>(FindChild(_Ty::GetTypeHashStatic(), childName, recursive)); }
	/// Find Childs of specified type, template version.
	template <typename _Ty> void FindChildren(TVector<_Ty*>& result, bool recursive = false) const { return FindChildren(reinterpret_cast<TVector<AActor*>&>(result), _Ty::GetTypeHashStatic(), recursive); }

	/// Has actor begun play.
	bool HasBegunPlay() { return _actorHasBegunPlay; }
private:
	/// Actor child actors.
	TVector<AActor*> _children;
	/// This actor all components.
	THashSet<AComponent*> _ownedComponents;
	/// Actor has begin palu.
	bool _actorHasBegunPlay;
};
}