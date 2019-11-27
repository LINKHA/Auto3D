#include "Debug/Log.h"
#include "IO/Stream.h"
#include "Core/ObjectResolver.h"
#include "Resource/JSONFile.h"
#include "Scene2D.h"
#include "Node2D.h"
#include "Camera2D.h"

#include "Debug/DebugNew.h"
namespace Auto3D
{

Node2D::Node2D():
	_flags(NF_2D_ENABLED),
	_layer(LAYER_2D_DEFAULT),
	_tag(TAG_2D_NONE),
	_parent(nullptr),
	_scene2D(nullptr),
	_id(0)
{

}

Node2D::~Node2D()
{
	RemoveAllChildren();
	// At the time of destruction the node should not have a parent, or be in a scene
	assert(!_parent);
	assert(!_scene2D);
}

void Node2D::RegisterObject()
{
	RegisterFactory<Node2D>();
}


void Node2D::Load(Stream& source, ObjectResolver& resolver)
{
	// Type and _id has been read by the parent
	Serializable::Load(source, resolver);

	size_t numChildren = source.ReadVLE();
	for (size_t i = 0; i < numChildren; ++i)
	{
		StringHash childType(source.Read<StringHash>());
		unsigned childId = source.Read<unsigned>();
		Node2D* child = CreateChild(childType);
		if (child)
		{
			resolver.StoreObject(childId, child);
			child->Load(source, resolver);
		}
		else
		{
			// If child is unknown type, skip all its attributes and children
			SkipHierarchy(source);
		}
	}
}

void Node2D::Save(Stream& dest)
{
	// Write type and ID first, followed by attributes and child nodes
	dest.Write(GetType());
	dest.Write(Id());
	Serializable::Save(dest);
	dest.WriteVLE(NumPersistentChildren());

	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (!child->IsTemporary())
			child->Save(dest);
	}
}

void Node2D::LoadJSON(const JSONValue& source, ObjectResolver& resolver)
{
	// Type and _id has been read by the parent
	Serializable::LoadJSON(source, resolver);

	const JSONArray& children = source["children"].GetArray();
	if (children.Size())
	{
		for (auto it = children.Begin(); it != children.End(); ++it)
		{
			const JSONValue& childJSON = *it;
			StringHash childType(childJSON["type"].GetString());
			unsigned childId = (unsigned)childJSON["id"].GetNumber();
			Node2D* child = CreateChild(childType);
			if (child)
			{
				resolver.StoreObject(childId, child);
				child->LoadJSON(childJSON, resolver);
			}
		}
	}
}

void Node2D::SaveJSON(JSONValue& dest)
{
	dest["type"] = GetTypeName();
	dest["id"] = Id();
	Serializable::SaveJSON(dest);

	if (NumPersistentChildren())
	{
		dest["children"].SetEmptyArray();
		for (auto it = _children.Begin(); it != _children.End(); ++it)
		{
			Node2D* child = *it;
			if (!child->IsTemporary())
			{
				JSONValue childJSON;
				child->SaveJSON(childJSON);
				dest["children"].Push(childJSON);
			}
		}
	}
}

bool Node2D::SaveJSON(Stream& dest)
{
	JSONFile json;
	SaveJSON(json.Root());
	return json.Save(dest);
}

void Node2D::SetName(const String& newName)
{
	SetName(newName.CString());
}

void Node2D::SetName(const char* newName)
{
	_name = newName;
}

void Node2D::SetLayer(unsigned char newLayer)
{
	if (_layer < 32)
		_layer = newLayer;
	else
		ErrorString("Can not set layer 32 or higher");
}

void Node2D::SetLayerName(const String& newLayerName)
{
	if (!_scene2D)
		return;

	const HashMap<String, unsigned char>& layers = _scene2D->Layers();
	auto it = layers.Find(newLayerName);
	if (it != layers.End())
		_layer = it->_second;
	else
		ErrorString("Layer " + newLayerName + " not defined in the scene");
}

void Node2D::SetTag(unsigned char newTag)
{
	_tag = newTag;
}

void Node2D::SetTagName(const String& newTagName)
{
	if (!_scene2D)
		return;

	const HashMap<String, unsigned char>& tags = _scene2D->Tags();
	auto it = tags.Find(newTagName);
	if (it != tags.End())
		_tag = it->_second;
	else
		ErrorString("Tag " + newTagName + " not defined in the scene");
}

void Node2D::SetEnabled(bool enable)
{
	SetFlag(NF_2D_ENABLED, enable);
	OnSetEnabled(TestFlag(NF_2D_ENABLED));
}

void Node2D::SetEnabledRecursive(bool enable)
{
	SetEnabled(enable);
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		child->SetEnabledRecursive(enable);
	}
}

void Node2D::SetTemporary(bool enable)
{
	SetFlag(NF_2D_TEMPORARY, enable);
}

void Node2D::SetParent(Node2D* newParent)
{
	if (newParent)
		newParent->AddChild(this);
	else
		ErrorString("Could not set null parent");
}

void Node2D::DefineLayer(unsigned char index, const String& name)
{
	if (index >= 32)
	{
		ErrorString("Can not define more than 32 layers");
		return;
	}

	if (_layerNames.Size() <= index)
		_layerNames.Resize(index + 1);
	_layerNames[index] = name;
	_layers[name] = index;
}

void Node2D::DefineTag(unsigned char index, const String& name)
{
	if (_tagNames.Size() <= index)
		_tagNames.Resize(index + 1);
	_tagNames[index] = name;
	_tags[name] = index;
}

Node2D* Node2D::CreateChild(StringHash childType)
{
	SharedPtr<Object> newObject = Create(childType);
	if (!newObject)
	{
		ErrorString("Could not create child node of unknown type " + childType.ToString());
		return nullptr;
	}
	Node2D* child = dynamic_cast<Node2D*>(newObject.Get());
	if (!child)
	{
		ErrorString(newObject->GetTypeName() + " is not a UINode subclass, could not add as a child");
		return nullptr;
	}

	AddChild(child);
	child->ParentCallBack();

	return child;
}

Node2D* Node2D::CreateChild(StringHash childType, const String& childName)
{
	return CreateChild(childType, childName.CString());
}

Node2D* Node2D::CreateChild(StringHash childType, const char* childName)
{
	Node2D* child = CreateChild(childType);
	if (child)
		child->SetName(childName);
	return child;
}

void Node2D::AddChild(Node2D* child)
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
	Node2D* current = _parent;
	while (current)
	{
		if (current == child)
		{
			ErrorString("Attempted cyclic node parenting");
			return;
		}
		current = current->_parent;
	}

	Node2D* oldParent = child->_parent;
	if (oldParent)
		oldParent->_children.Remove(child);
	_children.Push(child);
	child->_parent = this;
	child->OnParentSet(this, oldParent);

	if (_scene2D)
	{
		_scene2D->AddNode(child);
		if (child->GetType() == Camera2D::GetTypeStatic())
		{
			_scene2D->AddCamera(dynamic_cast<Camera2D*>(child));
		}
	}
}

void Node2D::RemoveChild(Node2D* child)
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

void Node2D::RemoveChild(size_t index)
{
	if (index >= _children.Size())
		return;

	Node2D* child = _children[index];
	// Detach from both the parent and the scene (removes _id assignment)
	child->_parent = nullptr;
	child->OnParentSet(this, nullptr);
	if (_scene2D)
		_scene2D->RemoveNode(child);
	_children.Erase(index);
}

void Node2D::RemoveAllChildren()
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		child->_parent = nullptr;
		child->OnParentSet(this, nullptr);
		if (_scene2D)
			_scene2D->RemoveNode(child);
		it->Reset();
	}

	_children.Clear();
}

void Node2D::RemoveSelf()
{
	if (_parent)
		_parent->RemoveChild(this);
	else
		delete this;
}

const String& Node2D::GetLayerName() const
{
	if (!_scene2D)
		return String::EMPTY;

	const Vector<String>& layerNames = _scene2D->LayerNames();
	return _layer < layerNames.Size() ? layerNames[_layer] : String::EMPTY;
}

const String& Node2D::GetTagName() const
{
	if (!_scene2D)
		return String::EMPTY;

	const Vector<String>& tagNames = _scene2D->TagNames();
	return _tag < tagNames.Size() ? tagNames[_layer] : String::EMPTY;
}

size_t Node2D::NumPersistentChildren() const
{
	size_t ret = 0;

	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (!child->IsTemporary())
			++ret;
	}

	return ret;
}

void Node2D::AllChildren(Vector<Node2D*>& result) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		result.Push(child);
		child->AllChildren(result);
	}
}

Node2D* Node2D::FindChild(const String& childName, bool recursive) const
{
	return FindChild(childName.CString(), recursive);
}

Node2D* Node2D::FindChild(const char* childName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (child->_name == childName)
			return child;
		else if (recursive && child->_children.Size())
		{
			Node2D* childResult = child->FindChild(childName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

Node2D* Node2D::FindChild(StringHash childType, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (child->GetType() == childType)
			return child;
		else if (recursive && child->_children.Size())
		{
			Node2D* childResult = child->FindChild(childType, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

Node2D* Node2D::FindChild(StringHash childType, const String& childName, bool recursive) const
{
	return FindChild(childType, childName.CString(), recursive);
}

Node2D* Node2D::FindChild(StringHash childType, const char* childName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (child->GetType() == childType && child->_name == childName)
			return child;
		else if (recursive && child->_children.Size())
		{
			Node2D* childResult = child->FindChild(childType, childName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

Node2D* Node2D::FindChildByLayer(unsigned layerMask, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (child->GetLayerMask() && layerMask)
			return child;
		else if (recursive && child->_children.Size())
		{
			Node2D* childResult = child->FindChildByLayer(layerMask, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

Node2D* Node2D::FindChildByTag(unsigned char tag, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (child->_tag == tag)
			return child;
		else if (recursive && child->_children.Size())
		{
			Node2D* childResult = child->FindChildByTag(tag, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

Node2D* Node2D::FindChildByTag(const String& tagName, bool recursive) const
{
	return FindChildByTag(tagName.CString(), recursive);
}

Node2D* Node2D::FindChildByTag(const char* tagName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (!String::Compare(child->GetTagName().CString(), tagName))
			return child;
		else if (recursive && child->_children.Size())
		{
			Node2D* childResult = child->FindChildByTag(tagName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

void Node2D::FindChildren(Vector<Node2D*>& result, StringHash childType, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (child->GetType() == childType)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildren(result, childType, recursive);
	}
}

void Node2D::FindChildrenByLayer(Vector<Node2D*>& result, unsigned layerMask, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (child->GetLayerMask() & layerMask)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildrenByLayer(result, layerMask, recursive);
	}
}

void Node2D::FindChildrenByTag(Vector<Node2D*>& result, unsigned char tag, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (child->_tag == tag)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildrenByTag(result, tag, recursive);
	}
}

void Node2D::FindChildrenByTag(Vector<Node2D*>& result, const String& tagName, bool recursive) const
{
	FindChildrenByTag(result, tagName.CString(), recursive);
}

void Node2D::FindChildrenByTag(Vector<Node2D*>& result, const char* tagName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		Node2D* child = *it;
		if (!String::Compare(child->GetTagName().CString(), tagName))
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildrenByTag(result, tagName, recursive);
	}
}

void Node2D::SetScene2D(Scene2D* newScene2D)
{
	Scene2D* oldCanvas = _scene2D;
	_scene2D = newScene2D;
	OnScene2DSet(_scene2D, oldCanvas);
}

void Node2D::SetId(unsigned newId)
{
	_id = newId;
}

void Node2D::SkipHierarchy(Stream& source)
{
	Serializable::Skip(source);

	size_t numChildren = source.ReadVLE();
	for (size_t i = 0; i < numChildren; ++i)
	{
		source.Read<StringHash>(); // StringHash childType
		source.Read<unsigned>(); // unsigned childId
		SkipHierarchy(source);
	}
}

void Node2D::OnParentSet(Node2D*, Node2D*)
{
}

void Node2D::OnScene2DSet(Scene2D*, Scene2D*)
{
}

void Node2D::OnSetEnabled(bool)
{
}

}