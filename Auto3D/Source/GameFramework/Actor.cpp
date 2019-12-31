#include "Actor.h"

namespace Auto3D
{

AActor::AActor():
	_actorHasBegunPlay(false)
{

}

AActor::~AActor()
{

}
void AActor::BeginPlay()
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AComponent* comp = *it;
		if(comp->IsEnabled() && !comp->HasBegunPlay())
			comp->BeginPlay();
	}

	//Get all current world actors
	TVector<AActor*> children;
	GetChildren(children, true);
	for (auto it = children.Begin(); it != children.End(); ++it)
	{
		AActor* child = *it;
		if (child->IsEnabled() && !child->HasBegunPlay())
			child->BeginPlay();
	}

	_actorHasBegunPlay = true;
}

void AActor::Tick(float deltaSeconds)
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AComponent* comp = *it;
		if (comp->IsEnabled() && !comp->HasBegunPlay())
			comp->BeginPlay();

		if (comp->IsEnabled())
			comp->TickComponent(deltaSeconds);
	}

	//Get all current world actors
	TVector<AActor*> children;
	GetChildren(children, true);
	for (auto it = children.Begin(); it != children.End(); ++it)
	{
		AActor* child = *it;
		if (child->IsEnabled() && !child->HasBegunPlay())
			child->BeginPlay();

		if (child->IsEnabled())
			child->Tick(deltaSeconds);
	}
}

AActor* AActor::GetParent()
{
	return dynamic_cast<AActor*>(GetParentNode());
}

AComponent* AActor::CreateComponent(FStringHash childType)
{
	AComponent* component = dynamic_cast<AComponent*>(CreateChildNode(childType));

	if (component)
	{
		_ownedComponents.Insert(component);
	}
	else
	{
		ErrorString("Failed to create component, perhaps the input parameter component");
		return nullptr;
	}
	component->OnActorSet(this);
	return component;
}

AComponent* AActor::CreateComponent(FStringHash childType, const FString& childName)
{
	return CreateComponent(childType, childName);
}

AComponent* AActor::CreateComponent(FStringHash childType, const char* childName)
{
	AComponent* component = dynamic_cast<AComponent*>(CreateChildNode(childType,childName));

	if (component)
	{
		_ownedComponents.Insert(component);
	}
	else
	{
		ErrorString("Failed to create component, perhaps the input parameter component");
		return nullptr;
	}
	component->OnActorSet(this);
	return component;
}

void AActor::AddComponent(AComponent* component)
{
	if (component)
	{
		_ownedComponents.Insert(component);
	}
	else
	{
		ErrorString("Failed to create component, perhaps the input parameter component");
		return;
	}
	AddChildNode(component);
	component->OnActorSet(this);
}

void AActor::RemoveComponent(AComponent* component)
{
	if (!component || component->GetParentNode() != this)
		return;

	if (_ownedComponents.Find(component) != _ownedComponents.End())
	{
		_ownedComponents.Erase(component);
		RemoveChildNode(component);
	}
	else
		WarningString("Fail remove component.");
}

void AActor::RemoveAllComponents()
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		RemoveChildNode(*it);
	}
	_ownedComponents.Clear();
}

void AActor::GetAllComponents(TVector<AComponent*>& result, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AComponent* comp = *it;
		result.Push(comp);
	}

	if (recursive)
	{
		for (auto it = _children.Begin(); it != _children.End(); ++it)
		{
			AActor* childActor = *it;
			childActor->GetAllComponents(result, recursive);
		}
	}
}

AComponent* AActor::FindComponent(const FString& childName, bool recursive) const
{
	return FindComponent(childName.CString(), recursive);
}

AComponent* AActor::FindComponent(const char* childName, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AComponent* comp = *it;
		if (comp->GetName() == childName)
			return comp;
	}

	if (recursive)
	{
		for (auto it = _children.Begin(); it != _children.End(); ++it)
		{
			AComponent* comp = (*it)->FindComponent(childName, recursive);
			if (comp)
				return comp;
		}
	}

	WarningStringF("Fail find component %s", childName);
	return nullptr;
}

AComponent* AActor::FindComponent(FStringHash childType, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AComponent* comp = *it;
		if (comp->GetTypeHash() == childType)
			return comp;
	}

	if (recursive)
	{
		for (auto it = _children.Begin(); it != _children.End(); ++it)
		{
			AComponent* comp = (*it)->FindComponent(childType, recursive);
			if (comp)
				return comp;
		}
	}

	WarningStringF("Fail find component %s", childType);
	return nullptr;
}

AComponent* AActor::FindComponent(FStringHash childType, const FString& childName, bool recursive) const
{
	return FindComponent(childType, childName.CString(), recursive);
}

AComponent* AActor::FindComponent(FStringHash childType, const char* childName, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AComponent* comp = *it;
		if (comp->GetTypeHash() == childType && comp->GetName() == childName)
			return comp;
	}

	if (recursive)
	{
		for (auto it = _children.Begin(); it != _children.End(); ++it)
		{
			AComponent* comp = (*it)->FindComponent(childType, recursive);
			if (comp)
				return comp;
		}
	}

	WarningStringF("Fail find component %s", childType);
	return nullptr;
}

void AActor::FindComponents(TVector<AComponent*>& result, FStringHash childType, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AComponent* comp = *it;
		if (comp->GetTypeHash() == childType)
			result.Push(comp);
	}
	if (recursive)
	{
		for (auto it = _children.Begin(); it != _children.End(); ++it)
		{
			(*it)->FindComponents(result, childType, recursive);
		}
	}
}

AActor* AActor::CreateChild(FStringHash childType)
{
	AActor* childActor = dynamic_cast<AActor*>(CreateChildNode(childType));

	if (childActor)
	{
		_children.Push(childActor);
	}
	else
	{
		ErrorString("Failed to create actor, perhaps the input parameter actor");
		return nullptr;
	}

	return childActor;
}

AActor* AActor::CreateChild(FStringHash childType, const FString& childName)
{
	return CreateChild(childType, childName.CString());
}

AActor* AActor::CreateChild(FStringHash childType, const char* childName)
{
	AActor* childActor = CreateChild(childType);
	if (childActor)
		childActor->SetName(childName);
	return childActor;
}

void AActor::AddChild(AActor* childActor)
{
	if (childActor)
	{
		_children.Push(childActor);
	}
	else
	{
		ErrorString("Failed to create actor, perhaps the input parameter component");
		return;
	}
	AddChildNode(childActor);
}

void AActor::RemoveChild(AActor* childActor)
{
	if (!childActor || childActor->GetParentNode() != this)
		return;

	for (size_t i = 0; i < _children.Size(); ++i)
	{
		if (_children[i] == childActor)
		{
			RemoveChild(i);
			break;
		}
	}
}

void AActor::RemoveChild(size_t index)
{
	if (index >= _children.Size())
		return;

	AActor* child = _children[index];
	_children.Erase(index);

	RemoveChildNode(child);
}

void AActor::RemoveAllChildren()
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		RemoveChildNode(*it);
	}
	_children.Clear();
}

void AActor::GetChildren(TVector<AActor*>& result, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		AActor* actor = *it;
		result.Push(actor);
		if (recursive)
			actor->GetChildren(result, recursive);
	}
}

AActor* AActor::FindChild(const FString& childName, bool recursive) const
{
	return FindChild(childName.CString(), recursive);
}

AActor* AActor::FindChild(const char* childName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		AActor* child = *it;
		if (child->GetName() == childName)
			return child;
		else if (recursive && child->_children.Size())
		{
			AActor* childResult = child->FindChild(childName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

AActor* AActor::FindChild(FStringHash childType, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		AActor* child = *it;
		if (child->GetTypeHash() == childType)
			return child;
		else if (recursive && child->_children.Size())
		{
			AActor* childResult = child->FindChild(childType, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

AActor* AActor::FindChild(FStringHash childType, const FString& childName, bool recursive) const
{
	return FindChild(childType, childName.CString(), recursive);
}

AActor* AActor::FindChild(FStringHash childType, const char* childName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		AActor* child = *it;
		if (child->GetTypeHash() == childType && child->_name == childName)
			return child;
		else if (recursive && child->_children.Size())
		{
			AActor* childResult = child->FindChild(childType, childName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

void AActor::FindChildren(TVector<AActor*>& result, FStringHash childType, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		AActor* child = *it;
		if (child->GetTypeHash() == childType)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildren(result, childType, recursive);
	}
}

}