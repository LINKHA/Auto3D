#include "Debug/Log.h"
#include "IO/Stream.h"
#include "Resource/JSONFile.h"
#include "Scene2D.h"
#include "Node2D.h"
#include "Camera2D.h"

#include "Debug/DebugNew.h"
namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(ANode2D)
	.constructor<>()
	.property("name", &ANode2D::GetName, &ANode2D::SetName)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("enabled", &ANode2D::IsEnabled, &ANode2D::SetEnabled)
	(
		metadata(SERIALIZABLE, true)
	)
	.property("temporary", &ANode2D::IsTemporary, &ANode2D::SetTemporary)
	(
		metadata(SERIALIZABLE, true)
	)
	.property("layer", &ANode2D::GetLayer, &ANode2D::SetLayer)
	(
		metadata(SERIALIZABLE, true)
	)
	.property("tag", &ANode2D::GetTag, &ANode2D::SetTag)
	(
		metadata(SERIALIZABLE, true)
	)
	;
}

ANode2D::ANode2D():
	_flags(NF_2D_ENABLED),
	_layer(LAYER_2D_DEFAULT),
	_tag(TAG_2D_NONE),
	_parent(nullptr),
	_scene2D(nullptr),
	_id(0)
{

}

ANode2D::~ANode2D()
{
	RemoveAllChildren();
	// At the time of destruction the node should not have a parent, or be in a scene
	assert(!_parent);
	assert(!_scene2D);
}

void ANode2D::RegisterObject()
{
	//RegisterFactory<ANode2D>();
}

void ANode2D::SetName(const FString& newName)
{
	if(!newName.IsEmpty())
		SetName(newName.CString());
}

void ANode2D::SetLayer(unsigned char newLayer)
{
	if (_layer < 32)
		_layer = newLayer;
	else
		ErrorString("Can not set layer 32 or higher");
}

void ANode2D::SetLayerName(const FString& newLayerName)
{
	if (!_scene2D)
		return;

	const THashMap<FString, unsigned char>& layers = _scene2D->Layers();
	auto it = layers.Find(newLayerName);
	if (it != layers.End())
		_layer = it->_second;
	else
		ErrorString("Layer " + newLayerName + " not defined in the scene");
}

void ANode2D::SetTag(unsigned char newTag)
{
	if (_tag < 32)
		_tag = newTag;
	else
		ErrorString("Can not set tag 32 or higher");
}

void ANode2D::SetTagName(const FString& newTagName)
{
	if (!_scene2D)
		return;

	const THashMap<FString, unsigned char>& tags = _scene2D->Tags();
	auto it = tags.Find(newTagName);
	if (it != tags.End())
		SetTag(it->_second);
	else
		ErrorString("Tag " + newTagName + " not defined in the scene");
}

void ANode2D::SetEnabled(bool enable)
{
	SetFlag(NF_2D_ENABLED, enable);
	OnSetEnabled(TestFlag(NF_2D_ENABLED));
}

void ANode2D::SetEnabledRecursive(bool enable)
{
	SetEnabled(enable);
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		child->SetEnabledRecursive(enable);
	}
}

void ANode2D::SetTemporary(bool enable)
{
	SetFlag(NF_2D_TEMPORARY, enable);
}

void ANode2D::SetParent(ANode2D* newParent)
{
	if (newParent)
		newParent->AddChild(this);
	else
		ErrorString("Could not set null parent");
}

ANode2D* ANode2D::CreateChild(FStringHash childType)
{
	TSharedPtr<AObject> newObject = Create(childType);
	if (!newObject)
	{
		ErrorString("Could not create child node of unknown type " + childType.ToString());
		return nullptr;
	}
	ANode2D* child = dynamic_cast<ANode2D*>(newObject.Get());
	if (!child)
	{
		ErrorString(newObject->GetTypeName() + " is not a UINode subclass, could not add as a child");
		return nullptr;
	}

	AddChild(child);
	child->ParentCallBack();

	return child;
}

ANode2D* ANode2D::CreateChild(FStringHash childType, const FString& childName)
{
	return CreateChild(childType, childName.CString());
}

ANode2D* ANode2D::CreateChild(FStringHash childType, const char* childName)
{
	ANode2D* child = CreateChild(childType);
	if (child)
		child->SetName(childName);
	return child;
}

void ANode2D::AddChild(ANode2D* child)
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
	ANode2D* current = _parent;
	while (current)
	{
		if (current == child)
		{
			ErrorString("Attempted cyclic node parenting");
			return;
		}
		current = current->_parent;
	}

	ANode2D* oldParent = child->_parent;
	if (oldParent)
		oldParent->_children.Remove(child);
	_children.Push(child);
	child->_parent = this;
	child->OnParentSet(this, oldParent);

	if (_scene2D)
	{
		_scene2D->AddNode(child);
		if (child->GetTypeHash() == ACamera2D::GetTypeHashStatic())
		{
			_scene2D->AddCamera(dynamic_cast<ACamera2D*>(child));
		}
	}
}

void ANode2D::RemoveChild(ANode2D* child)
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

void ANode2D::RemoveChild(size_t index)
{
	if (index >= _children.Size())
		return;

	ANode2D* child = _children[index];
	// Detach from both the parent and the scene (removes _id assignment)
	child->_parent = nullptr;
	child->OnParentSet(this, nullptr);
	if (_scene2D)
		_scene2D->RemoveNode(child);
	_children.Erase(index);
}

void ANode2D::RemoveAllChildren()
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		child->_parent = nullptr;
		child->OnParentSet(this, nullptr);
		if (_scene2D)
			_scene2D->RemoveNode(child);
		it->Reset();
	}

	_children.Clear();
}

void ANode2D::RemoveSelf()
{
	if (_parent)
		_parent->RemoveChild(this);
	else
		delete this;
}

const FString& ANode2D::GetLayerName() const
{
	if (!_scene2D)
		return FString::EMPTY;

	const THashMap<FString, unsigned char>& layers = _scene2D->Layers();

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

const FString& ANode2D::GetTagName() const
{
	if (!_scene2D)
		return FString::EMPTY;

	const THashMap<FString, unsigned char>& tags = _scene2D->Tags();

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

size_t ANode2D::NumPersistentChildren() const
{
	size_t ret = 0;

	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (!child->IsTemporary())
			++ret;
	}

	return ret;
}

void ANode2D::AllChildren(TVector<ANode2D*>& result) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		result.Push(child);
		child->AllChildren(result);
	}
}

ANode2D* ANode2D::FindChild(const FString& childName, bool recursive) const
{
	return FindChild(childName.CString(), recursive);
}

ANode2D* ANode2D::FindChild(const char* childName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (child->_name == childName)
			return child;
		else if (recursive && child->_children.Size())
		{
			ANode2D* childResult = child->FindChild(childName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

ANode2D* ANode2D::FindChild(FStringHash childType, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (child->GetTypeHash() == childType)
			return child;
		else if (recursive && child->_children.Size())
		{
			ANode2D* childResult = child->FindChild(childType, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

ANode2D* ANode2D::FindChild(FStringHash childType, const FString& childName, bool recursive) const
{
	return FindChild(childType, childName.CString(), recursive);
}

ANode2D* ANode2D::FindChild(FStringHash childType, const char* childName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (child->GetTypeHash() == childType && child->_name == childName)
			return child;
		else if (recursive && child->_children.Size())
		{
			ANode2D* childResult = child->FindChild(childType, childName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

ANode2D* ANode2D::FindChildByLayer(unsigned layerMask, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (child->GetLayerMask() && layerMask)
			return child;
		else if (recursive && child->_children.Size())
		{
			ANode2D* childResult = child->FindChildByLayer(layerMask, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

ANode2D* ANode2D::FindChildByTag(unsigned char tag, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (child->_tag == tag)
			return child;
		else if (recursive && child->_children.Size())
		{
			ANode2D* childResult = child->FindChildByTag(tag, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

ANode2D* ANode2D::FindChildByTag(const FString& tagName, bool recursive) const
{
	return FindChildByTag(tagName.CString(), recursive);
}

ANode2D* ANode2D::FindChildByTag(const char* tagName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (!FString::Compare(child->GetTagName().CString(), tagName))
			return child;
		else if (recursive && child->_children.Size())
		{
			ANode2D* childResult = child->FindChildByTag(tagName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

void ANode2D::FindChildren(TVector<ANode2D*>& result, FStringHash childType, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (child->GetTypeHash() == childType)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildren(result, childType, recursive);
	}
}

void ANode2D::FindChildrenByLayer(TVector<ANode2D*>& result, unsigned layerMask, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (child->GetLayerMask() & layerMask)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildrenByLayer(result, layerMask, recursive);
	}
}

void ANode2D::FindChildrenByTag(TVector<ANode2D*>& result, unsigned char tag, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (child->_tag == tag)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildrenByTag(result, tag, recursive);
	}
}

void ANode2D::FindChildrenByTag(TVector<ANode2D*>& result, const FString& tagName, bool recursive) const
{
	FindChildrenByTag(result, tagName.CString(), recursive);
}

void ANode2D::FindChildrenByTag(TVector<ANode2D*>& result, const char* tagName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		ANode2D* child = *it;
		if (!FString::Compare(child->GetTagName().CString(), tagName))
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildrenByTag(result, tagName, recursive);
	}
}

void ANode2D::SetScene2D(AScene2D* newScene2D)
{
	AScene2D* oldCanvas = _scene2D;
	_scene2D = newScene2D;
	OnScene2DSet(_scene2D, oldCanvas);
}

void ANode2D::SetId(unsigned newId)
{
	_id = newId;
}

}