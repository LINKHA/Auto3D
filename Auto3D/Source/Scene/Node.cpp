#include "Debug/Log.h"
#include "IO/Stream.h"
#include "Core/ObjectResolver.h"
#include "Resource/JSONFile.h"
#include "Engine/Components/Camera.h"
#include "Scene.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

REGISTER_CLASS
{
	using namespace rttr;
	FRegistration::class_<ANode>("ANode")
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
    _scenes(nullptr),
    _id(0)
{
}

ANode::~ANode()
{
    RemoveAllChildren();
    // At the time of destruction the node should not have a parent, or be in a scene
    assert(!_parent);
    assert(!_scenes);
}

void ANode::RegisterObject()
{
    RegisterFactory<ANode>();
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
    if (!_scenes)
        return;
    
	const THashMap<FString, unsigned char>& layers = _scenes->Layers();

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
	if (!_scenes)
		return;

	const THashMap<FString, unsigned char>& tags = _scenes->Tags();

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
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        child->SetEnabledRecursive(enable);
    }
}

void ANode::SetTemporary(bool enable)
{
    SetFlag(NF_TEMPORARY, enable);
}

void ANode::SetParent(ANode* newParent)
{
    if (newParent)
        newParent->AddChild(this);
    else
        ErrorString("Could not set null parent");
}

ANode* ANode::CreateChild(FStringHash childType)
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

    AddChild(child);
	child->ParentCallBack();

    return child;
}

ANode* ANode::CreateChild(FStringHash childType, const FString& childName)
{
    return CreateChild(childType, childName.CString());
}

ANode* ANode::CreateChild(FStringHash childType, const char* childName)
{
    ANode* child = CreateChild(childType);
    if (child)
        child->SetName(childName);
    return child;
}

void ANode::AddChild(ANode* child)
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
        oldParent->_children.Remove(child);
    _children.Push(child);
    child->_parent = this;
    child->OnParentSet(this, oldParent);
	if (_scenes)
	{
		_scenes->AddNode(child);
		if (child->GetType() == ACamera::GetTypeStatic())
		{
			_scenes->AddCamera(dynamic_cast<ACamera*>(child));
		}
	}
        
}

void ANode::RemoveChild(ANode* child)
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

void ANode::RemoveChild(size_t index)
{
    if (index >= _children.Size())
        return;

    ANode* child = _children[index];
    // Detach from both the parent and the scene (removes _id assignment)
    child->_parent = nullptr;
    child->OnParentSet(this, nullptr);
    if (_scenes)
        _scenes->RemoveNode(child);
    _children.Erase(index);
}

void ANode::RemoveAllChildren()
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        child->_parent = nullptr;
        child->OnParentSet(this, nullptr);
        if (_scenes)
            _scenes->RemoveNode(child);
        it->Reset();
    }

    _children.Clear();
}

void ANode::RemoveSelf()
{
    if (_parent)
        _parent->RemoveChild(this);
    else
        delete this;
}

const FString& ANode::GetLayerName() const
{
	if (!_scenes)
		return FString::EMPTY;

	const THashMap<FString, unsigned char>& layers = _scenes->Layers();

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
	if (!_scenes)
		return FString::EMPTY;

	const THashMap<FString, unsigned char>& tags = _scenes->Tags();

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

    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (!child->IsTemporary())
            ++ret;
    }

    return ret;
}

void ANode::AllChildren(TVector<ANode*>& result) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        result.Push(child);
        child->AllChildren(result);
    }
}

ANode* ANode::FindChild(const FString& childName, bool recursive) const
{
    return FindChild(childName.CString(), recursive);
}

ANode* ANode::FindChild(const char* childName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->_name == childName)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChild(childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChild(FStringHash childType, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetType() == childType)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChild(childType, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChild(FStringHash childType, const FString& childName, bool recursive) const
{
    return FindChild(childType, childName.CString(), recursive);
}

ANode* ANode::FindChild(FStringHash childType, const char* childName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetType() == childType && child->_name == childName)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChild(childType, childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildByLayer(unsigned layerMask, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetLayerMask() && layerMask)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChildByLayer(layerMask, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildByTag(unsigned char tag, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->_tag == tag)
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChildByTag(tag, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

ANode* ANode::FindChildByTag(const FString& tagName, bool recursive) const
{
    return FindChildByTag(tagName.CString(), recursive);
}

ANode* ANode::FindChildByTag(const char* tagName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            return child;
        else if (recursive && child->_children.Size())
        {
            ANode* childResult = child->FindChildByTag(tagName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

void ANode::FindChildren(TVector<ANode*>& result, FStringHash childType, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetType() == childType)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildren(result, childType, recursive);
    }
}

void ANode::FindChildrenByLayer(TVector<ANode*>& result, unsigned layerMask, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->GetLayerMask() & layerMask)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByLayer(result, layerMask, recursive);
    }
}

void ANode::FindChildrenByTag(TVector<ANode*>& result, unsigned char tag, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (child->_tag == tag)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByTag(result, tag, recursive);
    }
}

void ANode::FindChildrenByTag(TVector<ANode*>& result, const FString& tagName, bool recursive) const
{
    FindChildrenByTag(result, tagName.CString(), recursive);
}

void ANode::FindChildrenByTag(TVector<ANode*>& result, const char* tagName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        ANode* child = *it;
        if (!FString::Compare(child->GetTagName().CString(), tagName))
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByTag(result, tagName, recursive);
    }
}

void ANode::SetScene(AScene* newScene)
{
    AScene* oldScene = _scenes;
    _scenes = newScene;
    OnSceneSet(_scenes, oldScene);
}

void ANode::SetId(unsigned newId)
{
    _id = newId;
}

void ANode::SkipHierarchy(FStream& source)
{
    ASerializable::Skip(source);

    size_t numChildren = source.ReadVLE();
    for (size_t i = 0; i < numChildren; ++i)
    {
        source.Read<FStringHash>(); // FStringHash childType
        source.Read<unsigned>(); // unsigned childId
        SkipHierarchy(source);
    }
}

void ANode::OnParentSet(ANode*, ANode*)
{
}

void ANode::OnSceneSet(AScene*, AScene*)
{
}

void ANode::OnSetEnabled(bool)
{
}

}
