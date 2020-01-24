#include "Gameplay/Actor.h"
#include "Debug/Log.h"
#include "Container/HashMap.h"
#include "Component/ActorComponent.h"
#include "Component/Transform.h"
#include "Component/CameraComponent.h"

//#include "IO/Stream.h"
//#include "Resource/JSONFile.h"
#include "Gameplay/World.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

AActor::AActor() :
	_hasBegunPlay(false),
    _flags(NF_ENABLED),
    _layer(LAYER_DEFAULT),
    _tag(TAG_NONE),
    _parent(nullptr),
    _id(0)
{
	SetFlag(NF_SPATIAL, true);
}

AActor::~AActor()
{
    RemoveAllChildrenNode();
    // At the time of destruction the node should not have a parent, or be in a scene
    assert(!_parent);
    assert(!_world.lock());
}

void AActor::BeginPlay()
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		SPtr<AActorComponent> comp = it->_second;
		if (!comp->HasBegunPlay())
			comp->BeginPlay();
	}

	//Get all current world actors
	TVector<SPtr<AActor>> children;
	GetAllChildrenNode(children);

	for (auto it = children.Begin(); it != children.End(); ++it)
	{
		SPtr<AActor> child = *it;
		if (child->IsEnabled() && !child->HasBegunPlay())
			child->BeginPlay();
	}

	_hasBegunPlay = true;
}

void AActor::Tick(float deltaSeconds)
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		SPtr<AActorComponent> comp = it->_second;
		if (!comp->HasBegunPlay())
		{
			comp->BeginPlay();
		}
		comp->TickComponent(deltaSeconds);
	}

	//Get all current world actors
	TVector<SPtr<AActor>> children;
	GetAllChildrenNode(children);
	for (auto it = children.Begin(); it != children.End(); ++it)
	{
		SPtr<AActor> child = *it;
		if (child->IsEnabled() && !child->HasBegunPlay())
			child->BeginPlay();

		if (child->IsEnabled())
			child->Tick(deltaSeconds);
	}
}

void AActor::SetName(const FString& newName)
{
	if (!newName.IsEmpty())
		_name = newName;
}

void AActor::SetLayer(unsigned char newLayer)
{
    if (_layer < 32)
        _layer = newLayer;
    else
        ErrorString("Can not set layer 32 or higher");
}

void AActor::SetLayerName(const FString& newLayerName)
{
    if (!_world.lock())
        return;
    
	/*const THashMap<FString, unsigned char>& layers = _world->Layers();

	auto it = layers.Find(newLayerName);
	if (it != layers.End())
		SetLayer(it->_second);
	else
		ErrorString("Layer " + newLayerName + " not defined in the scene");*/

}

void AActor::SetTag(unsigned char newTag)
{
	if (_tag < 32)
		_tag = newTag;
	else
		ErrorString("Can not set tag 32 or higher");
}

void AActor::SetTagName(const FString& newTagName)
{
	if (!_world.lock())
		return;

	/*const THashMap<FString, unsigned char>& tags = _world->Tags();

	auto it = tags.Find(newTagName);
	if (it != tags.End())
		SetTag(it->_second);
	else
		ErrorString("Tag " + newTagName + " not defined in the scene");*/

}

void AActor::SetEnabled(bool enable)
{
    SetFlag(NF_ENABLED, enable);
    OnSetEnabled(TestFlag(NF_ENABLED));
}

void AActor::SetEnabledRecursive(bool enable)
{
    SetEnabled(enable);
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        child->SetEnabledRecursive(enable);
    }
}

void AActor::SetTemporary(bool enable)
{
    SetFlag(NF_TEMPORARY, enable);
}

void AActor::SetParentNode(SPtr<AActor> newParent)
{
    if (newParent)
        newParent->AddChildNode(SPtrThis());
    else
        ErrorString("Could not set null parent");
}

SPtr<AActor> AActor::CreateChildNode(FString childType)
{
	FType classType = FType::get_by_name(childType.CString());

	FVariant newObject = classType.create();
    if (!newObject)
    {
        ErrorString("Could not create child node of unknown type " + childType.ToString());
        return SPtr<AActor>();
    }

	// Classes created in FVariant are destructed in this scope, so copies are required
	SPtr<AActor> child;
	SPtr<AActor> tempChild = newObject.get_value<SPtr<AActor>>();
	memcpy(&child, &tempChild, sizeof(tempChild));

    if (!child.get())
    {
        ErrorString("Class is not a Node subclass, could not add as a child");
        return SPtr<AActor>();
    }

    AddChildNode(child);

    return child;
}

SPtr<AActor> AActor::CreateChildNode(FString childType, const FString& childName)
{
    return CreateChildNode(childType, childName.CString());
}

SPtr<AActor> AActor::CreateChildNode(FString childType, const char* childName)
{
	SPtr<AActor> child = CreateChildNode(childType);
    if (child)
        child->SetName(childName);
    return child;
}

void AActor::AddChildNode(SPtr<AActor> child)
{
    // Check for illegal or redundant parent assignment
    if (!child || child->_parent == SPtrThis())
        return;

    if (child == SPtrThis())
    {
        ErrorString("Attempted parenting node to self");
        return;
    }

    // Check for possible cyclic parent assignment
    SPtr<AActor> current = _parent;
    while (current)
    {
        if (current == child)
        {
            ErrorString("Attempted cyclic node parenting");
            return;
        }
        current = current->_parent;
    }

    SPtr<AActor> oldParent = child->_parent;
    if (oldParent)
        oldParent->_childrenNode.Remove(child);

    _childrenNode.Push(child);
    child->_parent = SPtrThis();
    child->OnParentSet(SPtrThis(), oldParent);

	if (_world.lock())
		_world.lock()->AddActor(child);
}

void AActor::RemoveChildNode(SPtr<AActor> child)
{
    if (!child || child->_parent != SPtrThis())
        return;

    for (size_t i = 0; i < _childrenNode.Size(); ++i)
    {
        if (_childrenNode[i] == child)
        {
            RemoveChildNode(i);
            break;
        }
    }
}

void AActor::RemoveChildNode(size_t index)
{
    //if (index >= _childrenNode.Size())
    //    return;

    //SPtr<AActor> child = _childrenNode[index];
    //// Detach from both the parent and the scene (removes _id assignment)
    //child->_parent = nullptr;
    //child->OnParentSet(this, nullptr);
    //if (_world)
    //    _world->RemoveNode(child);
    //_childrenNode.Erase(index);
}

void AActor::RemoveAllChildrenNode()
{
	/*for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
	{
		SPtr<AActor> child = *it;
		child->_parent = nullptr;
		child->OnParentSet(SPtrThis(), nullptr);
		if (_world)
			_world->RemoveNode(child);
		it->Reset();
	}
*/
    _childrenNode.Clear();
}

void AActor::RemoveSelf()
{
    if (_parent)
        _parent->RemoveChildNode(SPtrThis());
    else
        delete this;
}

const FString& AActor::GetLayerName() const
{
	//if (!_world)
	//	return FString::EMPTY;

	//const THashMap<FString, unsigned char>& layers = _world->Layers();

	//// Find value with layouts.
	//for (auto it = layers.Begin(); it != layers.End(); ++it)
	//{
	//	if (it->_second == _layer)
	//	{
	//		return it->_first;
	//	}
	//}

	//ErrorString("Fail find this layer from scene define layers");
	return FString::EMPTY;
}

const FString& AActor::GetTagName() const
{
	//if (!_world)
	//	return FString::EMPTY;

	//const THashMap<FString, unsigned char>& tags = _world->Tags();

	//// Find value with tags.
	//for (auto it = tags.Begin(); it != tags.End(); ++it)
	//{
	//	if (it->_second == _tag)
	//	{
	//		return it->_first;
	//	}
	//}

	//ErrorString("Fail find this tag from scene define tags");
	return FString::EMPTY;
}

size_t AActor::NumPersistentChildren() const
{
    size_t ret = 0;

    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        if (!child->IsTemporary())
            ++ret;
    }

    return ret;
}

void AActor::GetAllChildrenNode(TVector<SPtr<AActor>>& result, bool recursive) const
{
	if (!recursive)
	{
		result = _childrenNode;
		return;
	}
		
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        result.Push(child);
		if (recursive)
			child->GetAllChildrenNode(result, recursive);
    }
}

SPtr<AActor> AActor::FindChildNodeByName(const FString& childName, bool recursive) const
{
    return FindChildNodeByName(childName.CString(), recursive);
}

SPtr<AActor> AActor::FindChildNodeByName(const char* childName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        if (child->GetName() == FString(childName))
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            SPtr<AActor> childResult = child->FindChildNodeByName(childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return SPtr<AActor>();
}

SPtr<AActor> AActor::FindChildNodeByType(FString childType, bool recursive) const
{
	for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
	{
		SPtr<AActor> child = *it;
		if (FType::get(child) == FType::get_by_name(childType.CString()))
			return child;
		else if (recursive && child->_childrenNode.Size())
		{
			SPtr<AActor> childResult = child->FindChildNodeByType(childType, recursive);
			if (childResult)
				return childResult;
		}
	}

    return SPtr<AActor>();
}

SPtr<AActor> AActor::FindChildNodeByType(FString childType, const FString& childName, bool recursive) const
{
    return FindChildNodeByType(childType, childName.CString(), recursive);
}

SPtr<AActor> AActor::FindChildNodeByType(FString childType, const char* childName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        if (FType::get(child) == FType::get_by_name(childType.CString()) && child->GetName() == FString(childName))
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            SPtr<AActor> childResult = child->FindChildNodeByType(childType, childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return SPtr<AActor>();
}

SPtr<AActor> AActor::FindChildNodeByLayer(unsigned layerMask, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        if (child->GetLayerMask() && layerMask)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            SPtr<AActor> childResult = child->FindChildNodeByLayer(layerMask, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

SPtr<AActor> AActor::FindChildNodeByTag(unsigned char tag, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        if (child->_tag == tag)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            SPtr<AActor> childResult = child->FindChildNodeByTag(tag, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

SPtr<AActor> AActor::FindChildNodeByTag(const FString& tagName, bool recursive) const
{
    return FindChildNodeByTag(tagName.CString(), recursive);
}

SPtr<AActor> AActor::FindChildNodeByTag(const char* tagName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            SPtr<AActor> childResult = child->FindChildNodeByTag(tagName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

void AActor::FindChildrenNode(TVector<SPtr<AActor>>& result, FStringHash childType, bool recursive) const
{
	/* for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
	 {
		 SPtr<AActor> child = *it;
		 if (child->GetTypeHash() == childType)
			 result.Push(child);
		 if (recursive && child->_childrenNode.Size())
			 child->FindChildrenNode(result, childType, recursive);
	 }*/
}

void AActor::FindChildrenNodeByLayer(TVector<SPtr<AActor>>& result, unsigned layerMask, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        if (child->GetLayerMask() & layerMask)
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNodeByLayer(result, layerMask, recursive);
    }
}

void AActor::FindChildrenNodeByTag(TVector<SPtr<AActor>>& result, unsigned char tag, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        if (child->_tag == tag)
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNodeByTag(result, tag, recursive);
    }
}

void AActor::FindChildrenNodeByTag(TVector<SPtr<AActor>>& result, const FString& tagName, bool recursive) const
{
    FindChildrenNodeByTag(result, tagName.CString(), recursive);
}

void AActor::FindChildrenNodeByTag(TVector<SPtr<AActor>>& result, const char* tagName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        SPtr<AActor> child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNodeByTag(result, tagName, recursive);
    }
}

SPtr<AActorComponent> AActor::CreateComponent(FString childType)
{
	FType classType = FType::get_by_name(childType.CString());

	FVariant newObject = classType.create();

	// Classes created in FVariant are destructed in this scope, so copies are required
	SPtr<AActorComponent> component;
	SPtr<AActorComponent> tempComponent = newObject.get_value<SPtr<AActorComponent>>();
	memcpy(&component, &tempComponent, sizeof(tempComponent));

	AddComponent(component);

	return component;
}

void AActor::AddComponent(SPtr<AActorComponent> component)
{
	if (component)
	{
		_ownedComponents[RtToStr(FType::get(*component).get_name())] = component;

		if (FType::get(*component) == FType::get<ACameraComponent>())
		{
			if (_world.lock())
			{
				_world.lock()->AddCamera(DynamicCast<ACameraComponent>(component));
			}
			else
			{
				ErrorString("The Actor is attach world")
					return;
			}
		}

	}
	else
	{
		ErrorString("Failed to create component, perhaps the input parameter component");
		return;
	}
	component->AttachToActor(SPtrThis());
}

void AActor::RemoveComponent(SPtr<AActorComponent> component)
{
	FString componentsKey = RtToStr(FType::get(component).get_name());
	if (!component)
		return;

	if (_ownedComponents.Find(componentsKey) != _ownedComponents.End())
	{
		_ownedComponents.Erase(componentsKey);
	}
	else
		WarningString("Fail remove component.");
}

void AActor::RemoveAllComponents()
{
	_ownedComponents.Clear();
}

void AActor::GetAllComponents(TVector<SPtr<AActorComponent>>& result, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		SPtr<AActorComponent> comp = it->_second;
		result.Push(comp);
	}

	if (recursive)
	{
		for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
		{
			SPtr<AActor> childActor = *it;
			childActor->GetAllComponents(result, recursive);
		}
	}
}

SPtr<AActorComponent> AActor::FindComponent(FString childType, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		SPtr<AActorComponent> comp = it->_second;
		if (FType::get(comp) == FType::get_by_name(childType.CString()))
			return comp;
	}

	if (recursive)
	{
		for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
		{
			SPtr<AActorComponent> comp = (*it)->FindComponent(childType, recursive);
			if (comp)
				return comp;
		}
	}

	WarningStringF("Fail find component %s", childType);
	return nullptr;
}

void AActor::FindComponents(TVector<SPtr<AActorComponent>>& result, FString childType, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		SPtr<AActorComponent> comp = it->_second;
		if (FType::get(comp) == FType::get_by_name(childType.CString()))
			result.Push(comp);
	}
	if (recursive)
	{
		for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
		{
			(*it)->FindComponents(result, childType, recursive);
		}
	}
}

void AActor::SetWorld(SPtr<AWorld> newWorld)
{
	SPtr<AWorld> oldScene = _world.lock();
    _world = newWorld;
    OnWorldSet(newWorld, oldScene);
}

void AActor::SetId(unsigned newId)
{
    _id = newId;
}
SPtr<ATransform> AActor::GetTransform()
{ 
	if (!_transform.get())
	{
		CreateComponent<ATransform>();
	}

	return _transform;
}
}
