#include "Gameplay/Actor.h"
#include "Debug/Log.h"
#include "Container/HashMap.h"
#include "Component/ActorComponent.h"
#include "Component/Transform.h"
#include "Component/CameraComponent.h"

#include "IO/Stream.h"
#include "IO/JSONFile.h"

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
    _id(0),
	_world(nullptr)
{
	SetFlag(NF_SPATIAL, true);
}

AActor::~AActor()
{
    RemoveAllChildren();
    // At the time of destruction the node should not have a parent, or be in a scene
    assert(!_parent);
    assert(!_world);
}

void AActor::BeginPlay()
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AActorComponent* comp = it->_second;
		if (!comp->HasBegunPlay())
			comp->BeginPlay();
	}

	//Get all current world actors
	TVector<AActor*> children;
	GetAllChildren(children);

	for (auto it = children.Begin(); it != children.End(); ++it)
	{
		AActor* child = *it;
		if (child->IsEnabled() && !child->HasBegunPlay())
			child->BeginPlay();
	}

	_hasBegunPlay = true;
}

void AActor::Tick(float deltaSeconds)
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AActorComponent* comp = it->_second;
		if (!comp->HasBegunPlay())
		{
			comp->BeginPlay();
		}
		comp->TickComponent(deltaSeconds);
	}

	//Get all current world actors
	TVector<AActor*> children;
	GetAllChildren(children);
	for (auto it = children.Begin(); it != children.End(); ++it)
	{
		AActor* child = *it;
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
    if (!_world)
        return;
    
	const THashMap<FString, unsigned char>& layers = _world->Layers();

	auto it = layers.Find(newLayerName);
	if (it != layers.End())
		SetLayer(it->_second);
	else
		ErrorString("Layer " + newLayerName + " not defined in the scene");

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
	if (!_world)
		return;

	const THashMap<FString, unsigned char>& tags = _world->Tags();

	auto it = tags.Find(newTagName);
	if (it != tags.End())
		SetTag(it->_second);
	else
		ErrorString("Tag " + newTagName + " not defined in the scene");

}

void AActor::SetEnabled(bool enable)
{
    SetFlag(NF_ENABLED, enable);
    OnSetEnabled(TestFlag(NF_ENABLED));
}

void AActor::SetEnabledRecursive(bool enable)
{
    SetEnabled(enable);
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        child->SetEnabledRecursive(enable);
    }
}

void AActor::SetTemporary(bool enable)
{
    SetFlag(NF_TEMPORARY, enable);
}

void AActor::SetParentNode(AActor* newParent)
{
    if (newParent)
        newParent->AddChild(this);
    else
        ErrorString("Could not set null parent");
}

AActor* AActor::CreateChild(FString childType)
{
	OObject* newObject = NewObject(childType);

	if (!newObject)
	{
		ErrorString("Could not create child node of unknown type " + childType.ToString());
		return nullptr;
	}

	// Classes created in FVariant are destructed in this scope, so copies are required
	AActor* child = dynamic_cast<AActor*>(newObject);
    if (!child)
    {
        ErrorString("Class is not a Node subclass, could not add as a child");
        return nullptr;
    }

    AddChild(child);

    return child;
}

AActor* AActor::CreateChild(FString childType, const FString& childName)
{
    return CreateChild(childType, childName.CString());
}

AActor* AActor::CreateChild(FString childType, const char* childName)
{
	AActor* child = CreateChild(childType);
    if (child)
        child->SetName(childName);
    return child;
}

void AActor::AddChild(AActor* child)
{
    // Check for illegal or redundant parent assignment
    if (!child || child->_parent == this)
        return;

    if (child == this)
    {
        ErrorString("Attempted parenting node to self");
        return;
    }

    // Check for possible cyclic parent assignment
    AActor* current = _parent;
    while (current)
    {
        if (current == child)
        {
            ErrorString("Attempted cyclic node parenting");
            return;
        }
        current = current->_parent;
    }

	AActor* oldParent = child->_parent;
    if (oldParent)
        oldParent->_children.Remove(child);

    _children.Push(child);
    child->_parent = this;
    child->OnParentSet(this, oldParent);

	if (_world)
		_world->AddActor(child);
}

void AActor::RemoveChild(AActor* child)
{
    if (!child || child->_parent != this)
        return;

    for (size_t i = 0; i < _children.Size(); ++i)
    {
        if (_children[i] == child)
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
    // Detach from both the parent and the scene (removes _id assignment)
	if (_world)
		_world->RemoveActor(child);

    child->_parent = nullptr;
	child->_world = nullptr;
	child->OnParentSet(nullptr, nullptr);
	child->OnWorldSet(nullptr, nullptr);

    _children.Erase(index);

	SafeDelete(child);
}

void AActor::RemoveAllChildren()
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		AActor* child = *it;
		if (_world)
			_world->RemoveActor(child);

		child->_parent = nullptr;
		child->_world = nullptr;
		child->OnParentSet(nullptr, nullptr);
		child->OnWorldSet(nullptr, nullptr);

		SafeDelete(child);
	}

    _children.Clear();
}

void AActor::RemoveSelf()
{
	if (_parent)
		_parent->RemoveChild(this);
	else
		delete this;
}

const FString& AActor::GetLayerName() const
{
	if (!_world)
		return FString::EMPTY;

	const THashMap<FString, unsigned char>& layers = _world->Layers();

	// Find value with layouts.
	for (auto it = layers.Begin(); it != layers.End(); ++it)
	{
		if (it->_second == _layer)
		{
			return it->_first;
		}
	}

	ErrorString("Fail find this layer from scene define layers");
	return FString::EMPTY;
}

const FString& AActor::GetTagName() const
{
	if (!_world)
		return FString::EMPTY;

	const THashMap<FString, unsigned char>& tags = _world->Tags();

	// Find value with tags.
	for (auto it = tags.Begin(); it != tags.End(); ++it)
	{
		if (it->_second == _tag)
		{
			return it->_first;
		}
	}

	ErrorString("Fail find this tag from scene define tags");
	return FString::EMPTY;
}

size_t AActor::NumPersistentChildren() const
{
    size_t ret = 0;

    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        if (!child->IsTemporary())
            ++ret;
    }

    return ret;
}

void AActor::GetAllChildren(TVector<AActor*>& result, bool recursive) const
{
	if (!recursive)
	{
		result = _children;
		return;
	}
		
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        result.Push(child);
		if (recursive)
			child->GetAllChildren(result, recursive);
    }
}

AActor* AActor::FindChildByName(const FString& childName, bool recursive) const
{
    return FindChildByName(childName.CString(), recursive);
}

AActor* AActor::FindChildByName(const char* childName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        if (child->GetName() == FString(childName))
            return child;
        else if (recursive && child->_children.Size())
        {
            AActor* childResult = child->FindChildByName(childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

AActor* AActor::FindChildByType(FString childType, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		AActor* child = *it;
		if (FType::get(child) == FType::get_by_name(childType.CString()))
			return child;
		else if (recursive && child->_children.Size())
		{
			AActor* childResult = child->FindChildByType(childType, recursive);
			if (childResult)
				return childResult;
		}
	}

    return nullptr;
}

AActor* AActor::FindChildByType(FString childType, const FString& childName, bool recursive) const
{
    return FindChildByType(childType, childName.CString(), recursive);
}

AActor* AActor::FindChildByType(FString childType, const char* childName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        if (FType::get(child) == FType::get_by_name(childType.CString()) && child->GetName() == FString(childName))
            return child;
        else if (recursive && child->_children.Size())
        {
            AActor* childResult = child->FindChildByType(childType, childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

AActor* AActor::FindChildByLayer(unsigned layerMask, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        if (child->GetLayerMask() && layerMask)
            return child;
        else if (recursive && child->_children.Size())
        {
            AActor* childResult = child->FindChildByLayer(layerMask, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

AActor* AActor::FindChildByTag(unsigned char tag, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        if (child->_tag == tag)
            return child;
        else if (recursive && child->_children.Size())
        {
            AActor* childResult = child->FindChildByTag(tag, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

AActor* AActor::FindChildByTag(const FString& tagName, bool recursive) const
{
    return FindChildByTag(tagName.CString(), recursive);
}

AActor* AActor::FindChildByTag(const char* tagName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            return child;
        else if (recursive && child->_children.Size())
        {
            AActor* childResult = child->FindChildByTag(tagName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

void AActor::FindChildren(TVector<AActor*>& result, FString childType, bool recursive) const
{
	 for (auto it = _children.Begin(); it != _children.End(); ++it)
	 {
		 AActor* child = *it;
		 if (RtToStr(FType::get(child).get_name()) == childType)
			 result.Push(child);
		 if (recursive && child->_children.Size())
			 child->FindChildren(result, childType, recursive);
	 }
}

void AActor::FindChildrenByLayer(TVector<AActor*>& result, unsigned layerMask, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        if (child->GetLayerMask() & layerMask)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByLayer(result, layerMask, recursive);
    }
}

void AActor::FindChildrenByTag(TVector<AActor*>& result, unsigned char tag, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        if (child->_tag == tag)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByTag(result, tag, recursive);
    }
}

void AActor::FindChildrenByTag(TVector<AActor*>& result, const FString& tagName, bool recursive) const
{
    FindChildrenByTag(result, tagName.CString(), recursive);
}

void AActor::FindChildrenByTag(TVector<AActor*>& result, const char* tagName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        AActor* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByTag(result, tagName, recursive);
    }
}

AActorComponent* AActor::CreateComponent(FString childType)
{
	OObject* newObject = NewObject(childType);
	if (!newObject)
	{
		ErrorString("Could not create child node of unknown type " + childType.ToString());
		return nullptr;
	}

	AActorComponent* component = dynamic_cast<AActorComponent*>(newObject);
	if (!component)
	{
		ErrorString(newObject->GetTypeName() + " is not a Node subclass, could not add as a child");
		return nullptr;
	}

	AddComponent(component);

	return component;
}

void AActor::AddComponent(AActorComponent* component)
{
	if (component)
	{
		FString ss = RtToStr(FType::get(*component).get_name());

		_ownedComponents[RtToStr(FType::get(*component).get_name())] = component;

		if (FType::get(*component) == FType::get<ACameraComponent>())
		{
			if (_world)
			{
				_world->AddCamera(dynamic_cast<ACameraComponent*>(component));
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
	component->AttachToActor(this);
}

void AActor::RemoveComponent(AActorComponent* component)
{
	FString componentsKey = RtToStr(FType::get(component).get_name());
	if (!component)
		return;

	auto it = _ownedComponents.Find(componentsKey);
	if (it != _ownedComponents.End())
	{
		_ownedComponents.Erase(componentsKey);
		SafeDelete(it->_second);
	}
	else
		WarningString("Fail remove component.");
}

void AActor::RemoveAllComponents()
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AActorComponent* comp = it->_second;

		SafeDelete(comp);
	}

	_ownedComponents.Clear();
}

void AActor::GetAllComponents(TVector<AActorComponent*>& result, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AActorComponent* comp = it->_second;
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

AActorComponent* AActor::FindComponent(FString childType, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AActorComponent* comp = it->_second;
		if (FType::get(comp) == FType::get_by_name(childType.CString()))
			return comp;
	}

	if (recursive)
	{
		for (auto it = _children.Begin(); it != _children.End(); ++it)
		{
			AActorComponent* comp = (*it)->FindComponent(childType, recursive);
			if (comp)
				return comp;
		}
	}

	WarningStringF("Fail find component %s", childType);
	return nullptr;
}

void AActor::FindComponents(TVector<AActorComponent*>& result, FString childType, bool recursive) const
{
	for (auto it = _ownedComponents.Begin(); it != _ownedComponents.End(); ++it)
	{
		AActorComponent* comp = it->_second;
		if (FType::get(comp) == FType::get_by_name(childType.CString()))
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

void AActor::SetWorld(AWorld* newWorld)
{
	AWorld* oldScene = _world;
    _world = newWorld;
    OnWorldSet(newWorld, oldScene);
}

void AActor::SetId(unsigned newId)
{
    _id = newId;
}
ATransform* AActor::GetTransform()
{ 
	if (!_transform)
	{
		CreateComponent<ATransform>();
	}

	return _transform;
}
}
