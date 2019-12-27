#include "Debug/Log.h"
#include "IO/Stream.h"
#include "Resource/JSONFile.h"
#include "Engine/Components/Camera.h"
#include "Scene/World.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(ANode)
	.constructor<>()
	.property("name", &ANode::GetName, &ANode::SetName)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("enabled", &ANode::IsEnabled, &ANode::SetEnabled)
	(
		metadata(SERIALIZABLE, true)
	)
	.property("temporary", &ANode::IsTemporary, &ANode::SetTemporary)
	(
		metadata(SERIALIZABLE, true)
	)
	.property("layer", &ANode::GetLayer, &ANode::SetLayer)
	(
		metadata(SERIALIZABLE, true)
	)
	.property("tag", &ANode::GetTag, &ANode::SetTag)
	(
		metadata(SERIALIZABLE, true)
	)
	;
}

static TVector<TSharedPtr<ANode> > noChildren;

ANode::ANode() :
    _flags(NF_ENABLED),
    _layer(LAYER_DEFAULT),
    _tag(TAG_NONE),
    _parent(nullptr),
    _world(nullptr),
    _id(0)
{
}

ANode::~ANode()
{
    RemoveAllChildrenNode();
    // At the time of destruction the node should not have a parent, or be in a scene
    assert(!_parent);
    assert(!_world);
}

void ANode::SetName(const FString& newName)
{
	if(!newName.IsEmpty())
		SetName(newName.CString());
}

void ANode::SetLayer(unsigned char newLayer)
{
    if (_layer < 32)
        _layer = newLayer;
    else
        ErrorString("Can not set layer 32 or higher");
}

void ANode::SetLayerName(const FString& newLayerName)
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

void ANode::SetTag(unsigned char newTag)
{
	if (_tag < 32)
		_tag = newTag;
	else
		ErrorString("Can not set tag 32 or higher");
}

void ANode::SetTagName(const FString& newTagName)
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

void ANode::SetEnabled(bool enable)
{
    SetFlag(NF_ENABLED, enable);
    OnSetEnabled(TestFlag(NF_ENABLED));
}

void ANode::SetEnabledRecursive(bool enable)
{
    SetEnabled(enable);
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        child->SetEnabledRecursive(enable);
    }
}

void ANode::SetTemporary(bool enable)
{
    SetFlag(NF_TEMPORARY, enable);
}

void ANode::SetParentNode(ANode* newParent)
{
    if (newParent)
        newParent->AddChildNode(this);
    else
        ErrorString("Could not set null parent");
}

ANode* ANode::CreateChildNode(FStringHash childType)
{
    TSharedPtr<AObject> newObject = Create(childType);
    if (!newObject)
    {
        ErrorString("Could not create child node of unknown type " + childType.ToString());
        return nullptr;
    }
    ANode* child = dynamic_cast<ANode*>(newObject.Get());
    if (!child)
    {
        ErrorString(newObject->GetTypeName() + " is not a Node subclass, could not add as a child");
        return nullptr;
    }

    AddChildNode(child);

    return child;
}

ANode* ANode::CreateChildNode(FStringHash childType, const FString& childName)
{
    return CreateChildNode(childType, childName.CString());
}

ANode* ANode::CreateChildNode(FStringHash childType, const char* childName)
{
    ANode* child = CreateChildNode(childType);
    if (child)
        child->SetName(childName);
    return child;
}

void ANode::AddChildNode(ANode* child)
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
    ANode* current = _parent;
    while (current)
    {
        if (current == child)
        {
            ErrorString("Attempted cyclic node parenting");
            return;
        }
        current = current->_parent;
    }

    ANode* oldParent = child->_parent;
    if (oldParent)
        oldParent->_childrenNode.Remove(child);

    _childrenNode.Push(child);
    child->_parent = this;
    child->OnParentSet(this, oldParent);

	if (_world)
		_world->AddNode(child);
}

void ANode::RemoveChildNode(ANode* child)
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

void ANode::RemoveChildNode(size_t index)
{
    if (index >= _childrenNode.Size())
        return;

    ANode* child = _childrenNode[index];
    // Detach from both the parent and the scene (removes _id assignment)
    child->_parent = nullptr;
    child->OnParentSet(this, nullptr);
    if (_world)
        _world->RemoveNode(child);
    _childrenNode.Erase(index);
}

void ANode::RemoveAllChildrenNode()
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        child->_parent = nullptr;
        child->OnParentSet(this, nullptr);
        if (_world)
            _world->RemoveNode(child);
        it->Reset();
    }

    _childrenNode.Clear();
}

void ANode::RemoveSelf()
{
    if (_parent)
        _parent->RemoveChildNode(this);
    else
        delete this;
}

const FString& ANode::GetLayerName() const
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

const FString& ANode::GetTagName() const
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

size_t ANode::NumPersistentChildren() const
{
    size_t ret = 0;

    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (!child->IsTemporary())
            ++ret;
    }

    return ret;
}

void ANode::GetAllChildrenNode(TVector<ANode*>& result) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        result.Push(child);
        child->GetAllChildrenNode(result);
    }
}

ANode* ANode::FindChildNode(const FString& childName, bool recursive) const
{
    return FindChildNode(childName.CString(), recursive);
}

ANode* ANode::FindChildNode(const char* childName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (child->_name == childName)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            ANode* childResult = child->FindChildNode(childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildNode(FStringHash childType, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetTypeHash() == childType)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            ANode* childResult = child->FindChildNode(childType, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildNode(FStringHash childType, const FString& childName, bool recursive) const
{
    return FindChildNode(childType, childName.CString(), recursive);
}

ANode* ANode::FindChildNode(FStringHash childType, const char* childName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetTypeHash() == childType && child->_name == childName)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            ANode* childResult = child->FindChildNode(childType, childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildNodeByLayer(unsigned layerMask, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetLayerMask() && layerMask)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            ANode* childResult = child->FindChildNodeByLayer(layerMask, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildNodeByTag(unsigned char tag, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (child->_tag == tag)
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            ANode* childResult = child->FindChildNodeByTag(tag, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildNodeByTag(const FString& tagName, bool recursive) const
{
    return FindChildNodeByTag(tagName.CString(), recursive);
}

ANode* ANode::FindChildNodeByTag(const char* tagName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            return child;
        else if (recursive && child->_childrenNode.Size())
        {
            ANode* childResult = child->FindChildNodeByTag(tagName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

void ANode::FindChildrenNode(TVector<ANode*>& result, FStringHash childType, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetTypeHash() == childType)
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNode(result, childType, recursive);
    }
}

void ANode::FindChildrenNodeByLayer(TVector<ANode*>& result, unsigned layerMask, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetLayerMask() & layerMask)
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNodeByLayer(result, layerMask, recursive);
    }
}

void ANode::FindChildrenNodeByTag(TVector<ANode*>& result, unsigned char tag, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (child->_tag == tag)
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNodeByTag(result, tag, recursive);
    }
}

void ANode::FindChildrenNodeByTag(TVector<ANode*>& result, const FString& tagName, bool recursive) const
{
    FindChildrenNodeByTag(result, tagName.CString(), recursive);
}

void ANode::FindChildrenNodeByTag(TVector<ANode*>& result, const char* tagName, bool recursive) const
{
    for (auto it = _childrenNode.Begin(); it != _childrenNode.End(); ++it)
    {
        ANode* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            result.Push(child);
        if (recursive && child->_childrenNode.Size())
            child->FindChildrenNodeByTag(result, tagName, recursive);
    }
}

void ANode::SetScene(AWorld* newScene)
{
    AWorld* oldScene = _world;
    _world = newScene;
    OnWorldSet(_world, oldScene);
}

void ANode::SetId(unsigned newId)
{
    _id = newId;
}


}
