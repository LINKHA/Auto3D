#include "Gameplay/Actor.h"
#include "Debug/Log.h"
#include "Container/HashMap.h"

//#include "IO/Stream.h"
//#include "Resource/JSONFile.h"
#include "Gameplay/World.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

static TVector<std::shared_ptr<AActor> > noChildren;

AActor::AActor() :
    _flags(NF_ENABLED),
    _layer(LAYER_DEFAULT),
    _tag(TAG_NONE),
    _parent(nullptr),
    _world(nullptr),
    _id(0)
{
	SetFlag(NF_SPATIAL, true);
}

AActor::~AActor()
{
    RemoveAllChildrenNode();
    // At the time of destruction the node should not have a parent, or be in a scene
    assert(!_parent);
    assert(!_world);
}

void AActor::SetName(const FString& newName)
{
	if(!newName.IsEmpty())
		SetName(newName.CString());
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
	if (!_world)
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
        newParent->AddChildNode(this);
    else
        ErrorString("Could not set null parent");
}

AActor* AActor::CreateChildNode(FString childType)
{
	FType classType = FType::get_by_name(ToRtStr(childType));
	FVariant newObject = classType.create();

    if (!newObject)
    {
        ErrorString("Could not create child node of unknown type " + childType.ToString());
        return nullptr;
    }

    AActor* child = newObject.get_value<AActor*>();

    if (!child)
    {
        ErrorString(newObject->GetTypeName() + " is not a Node subclass, could not add as a child");
        return nullptr;
    }

    AddChildNode(child);

    return child;
}

//AActor* AActor::CreateChildNode(FStringHash childType, const FString& childName)
//{
//    return CreateChildNode(childType, childName.CString());
//}
//
//AActor* AActor::CreateChildNode(FStringHash childType, const char* childName)
//{
//    AActor* child = CreateChildNode(childType);
//    if (child)
//        child->SetName(childName);
//    return child;
//}

void AActor::AddChildNode(AActor* child)
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
        oldParent->_childrenNode.Remove(child);

    _childrenNode.Push(child);
    child->_parent = this;
    child->OnParentSet(this, oldParent);

	//if (_world)
	//	_world->AddNode(child);
}

void AActor::RemoveChildNode(AActor* child)
{
    if (!child || child->_parent != this)
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

    //AActor* child = _childrenNode[index];
    //// Detach from both the parent and the scene (removes _id assignment)
    //child->_parent = nullptr;
    //child->OnParentSet(this, nullptr);
    //if (_world)
    //    _world->RemoveNode(child);
    //_childrenNode.Erase(index);
}

void AActor::RemoveAllChildrenNode()
{
   /* for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        child->_parent = nullptr;
        child->OnParentSet(this, nullptr);
        if (_world)
            _world->RemoveNode(child);
        it->Reset();
    }*/

    _childrenNode.Clear();
}

void AActor::RemoveSelf()
{
    if (_parent)
        _parent->RemoveChildNode(this);
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
        AActor* child = *it;
        if (!child->IsTemporary())
            ++ret;
    }

    return ret;
}

void AActor::GetAllChildrenNode(TVector<AActor*>& result) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        result.Push(child);
        child->GetAllChildrenNode(result);
    }
}

AActor* AActor::FindChildNode(const FString& childName, bool recursive) const
{
    return FindChildNode(childName.CString(), recursive);
}

AActor* AActor::FindChildNode(const char* childName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        if (child->_name == childName)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            AActor* childResult = child->FindChildNode(childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

AActor* AActor::FindChildNode(FStringHash childType, bool recursive) const
{
	/*for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
	{
		AActor* child = *it;
		if (child->GetTypeHash() == childType)
			return child;
		else if (recursive && child->_childrenNode.Size())
		{
			AActor* childResult = child->FindChildNode(childType, recursive);
			if (childResult)
				return childResult;
		}
	}*/

    return nullptr;
}

AActor* AActor::FindChildNode(FStringHash childType, const FString& childName, bool recursive) const
{
    return FindChildNode(childType, childName.CString(), recursive);
}

AActor* AActor::FindChildNode(FStringHash childType, const char* childName, bool recursive) const
{
    /*for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        if (child->GetTypeHash() == childType && child->_name == childName)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            AActor* childResult = child->FindChildNode(childType, childName, recursive);
            if (childResult)
                return childResult;
        }
    }*/

    return nullptr;
}

AActor* AActor::FindChildNodeByLayer(unsigned layerMask, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        if (child->GetLayerMask() && layerMask)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            AActor* childResult = child->FindChildNodeByLayer(layerMask, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

AActor* AActor::FindChildNodeByTag(unsigned char tag, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        if (child->_tag == tag)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            AActor* childResult = child->FindChildNodeByTag(tag, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

AActor* AActor::FindChildNodeByTag(const FString& tagName, bool recursive) const
{
    return FindChildNodeByTag(tagName.CString(), recursive);
}

AActor* AActor::FindChildNodeByTag(const char* tagName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            AActor* childResult = child->FindChildNodeByTag(tagName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

void AActor::FindChildrenNode(TVector<AActor*>& result, FStringHash childType, bool recursive) const
{
	/* for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
	 {
		 AActor* child = *it;
		 if (child->GetTypeHash() == childType)
			 result.Push(child);
		 if (recursive && child->_childrenNode.Size())
			 child->FindChildrenNode(result, childType, recursive);
	 }*/
}

void AActor::FindChildrenNodeByLayer(TVector<AActor*>& result, unsigned layerMask, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        if (child->GetLayerMask() & layerMask)
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNodeByLayer(result, layerMask, recursive);
    }
}

void AActor::FindChildrenNodeByTag(TVector<AActor*>& result, unsigned char tag, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        if (child->_tag == tag)
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNodeByTag(result, tag, recursive);
    }
}

void AActor::FindChildrenNodeByTag(TVector<AActor*>& result, const FString& tagName, bool recursive) const
{
    FindChildrenNodeByTag(result, tagName.CString(), recursive);
}

void AActor::FindChildrenNodeByTag(TVector<AActor*>& result, const char* tagName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        AActor* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNodeByTag(result, tagName, recursive);
    }
}

void AActor::SetScene(AWorld* newScene)
{
    AWorld* oldScene = _world;
    _world = newScene;
    OnWorldSet(_world, oldScene);
}

void AActor::SetId(unsigned newId)
{
    _id = newId;
}

}
