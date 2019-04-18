#include "../Debug/Log.h"
#include "../IO/Stream.h"
#include "../Object/ObjectResolver.h"
#include "../Resource/JSONFile.h"
#include "Canvas.h"

#include "../Debug/DebugNew.h"
namespace Auto3D
{

UINode::UINode():
	_flags(UNF_ENABLED),
	_layer(U_LAYER_DEFAULT),
	_tag(U_TAG_NONE),
	_parent(nullptr),
	_canvas(nullptr),
	_id(0)
{

}

UINode::~UINode()
{
	RemoveAllChildren();
	// At the time of destruction the node should not have a parent, or be in a scene
	assert(!_parent);
	assert(!_canvas);
}

void UINode::RegisterObject()
{
	RegisterFactory<UINode>();
}


void UINode::Load(Stream& source, ObjectResolver& resolver)
{
	// Type and _id has been read by the parent
	Serializable::Load(source, resolver);

	size_t numChildren = source.ReadVLE();
	for (size_t i = 0; i < numChildren; ++i)
	{
		StringHash childType(source.Read<StringHash>());
		unsigned childId = source.Read<unsigned>();
		UINode* child = CreateChild(childType);
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

void UINode::Save(Stream& dest)
{
	// Write type and ID first, followed by attributes and child nodes
	dest.Write(GetType());
	dest.Write(Id());
	Serializable::Save(dest);
	dest.WriteVLE(NumPersistentChildren());

	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (!child->IsTemporary())
			child->Save(dest);
	}
}

void UINode::LoadJSON(const JSONValue& source, ObjectResolver& resolver)
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
			UINode* child = CreateChild(childType);
			if (child)
			{
				resolver.StoreObject(childId, child);
				child->LoadJSON(childJSON, resolver);
			}
		}
	}
}

void UINode::SaveJSON(JSONValue& dest)
{
	dest["type"] = GetTypeName();
	dest["id"] = Id();
	Serializable::SaveJSON(dest);

	if (NumPersistentChildren())
	{
		dest["children"].SetEmptyArray();
		for (auto it = _children.Begin(); it != _children.End(); ++it)
		{
			UINode* child = *it;
			if (!child->IsTemporary())
			{
				JSONValue childJSON;
				child->SaveJSON(childJSON);
				dest["children"].Push(childJSON);
			}
		}
	}
}

bool UINode::SaveJSON(Stream& dest)
{
	JSONFile json;
	SaveJSON(json.Root());
	return json.Save(dest);
}

void UINode::SetName(const String& newName)
{
	SetName(newName.CString());
}

void UINode::SetName(const char* newName)
{
	_name = newName;
}

void UINode::SetLayer(unsigned char newLayer)
{
	if (_layer < 32)
		_layer = newLayer;
	else
		ErrorString("Can not set layer 32 or higher");
}

void UINode::SetLayerName(const String& newLayerName)
{
	if (!_canvas)
		return;

	const HashMap<String, unsigned char>& layers = _canvas->Layers();
	auto it = layers.Find(newLayerName);
	if (it != layers.End())
		_layer = it->_second;
	else
		ErrorString("Layer " + newLayerName + " not defined in the scene");
}

void UINode::SetTag(unsigned char newTag)
{
	_tag = newTag;
}

void UINode::SetTagName(const String& newTagName)
{
	if (!_canvas)
		return;

	const HashMap<String, unsigned char>& tags = _canvas->Tags();
	auto it = tags.Find(newTagName);
	if (it != tags.End())
		_tag = it->_second;
	else
		ErrorString("Tag " + newTagName + " not defined in the scene");
}

void UINode::SetEnabled(bool enable)
{
	SetFlag(UNF_ENABLED, enable);
	OnSetEnabled(TestFlag(UNF_ENABLED));
}

void UINode::SetEnabledRecursive(bool enable)
{
	SetEnabled(enable);
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		child->SetEnabledRecursive(enable);
	}
}

void UINode::SetTemporary(bool enable)
{
	SetFlag(UNF_TEMPORARY, enable);
}

void UINode::SetParent(UINode* newParent)
{
	if (newParent)
		newParent->AddChild(this);
	else
		ErrorString("Could not set null parent");
}

void UINode::DefineLayer(unsigned char index, const String& name)
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

void UINode::DefineTag(unsigned char index, const String& name)
{
	if (_tagNames.Size() <= index)
		_tagNames.Resize(index + 1);
	_tagNames[index] = name;
	_tags[name] = index;
}

UINode* UINode::CreateChild(StringHash childType)
{
	SharedPtr<Object> newObject = Create(childType);
	if (!newObject)
	{
		ErrorString("Could not create child node of unknown type " + childType.ToString());
		return nullptr;
	}
	UINode* child = dynamic_cast<UINode*>(newObject.Get());
	if (!child)
	{
		ErrorString(newObject->GetTypeName() + " is not a UINode subclass, could not add as a child");
		return nullptr;
	}

	AddChild(child);
	return child;
}

UINode* UINode::CreateChild(StringHash childType, const String& childName)
{
	return CreateChild(childType, childName.CString());
}

UINode* UINode::CreateChild(StringHash childType, const char* childName)
{
	UINode* child = CreateChild(childType);
	if (child)
		child->SetName(childName);
	return child;
}

void UINode::AddChild(UINode* child)
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
	UINode* current = _parent;
	while (current)
	{
		if (current == child)
		{
			ErrorString("Attempted cyclic node parenting");
			return;
		}
		current = current->_parent;
	}

	UINode* oldParent = child->_parent;
	if (oldParent)
		oldParent->_children.Remove(child);
	_children.Push(child);
	child->_parent = this;
	child->OnParentSet(this, oldParent);
	if (_canvas)
		_canvas->AddNode(child);
}

void UINode::RemoveChild(UINode* child)
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

void UINode::RemoveChild(size_t index)
{
	if (index >= _children.Size())
		return;

	UINode* child = _children[index];
	// Detach from both the parent and the scene (removes _id assignment)
	child->_parent = nullptr;
	child->OnParentSet(this, nullptr);
	if (_canvas)
		_canvas->RemoveNode(child);
	_children.Erase(index);
}

void UINode::RemoveAllChildren()
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		child->_parent = nullptr;
		child->OnParentSet(this, nullptr);
		if (_canvas)
			_canvas->RemoveNode(child);
		it->Reset();
	}

	_children.Clear();
}

void UINode::RemoveSelf()
{
	if (_parent)
		_parent->RemoveChild(this);
	else
		delete this;
}

const String& UINode::GetLayerName() const
{
	if (!_canvas)
		return String::EMPTY;

	const Vector<String>& layerNames = _canvas->LayerNames();
	return _layer < layerNames.Size() ? layerNames[_layer] : String::EMPTY;
}

const String& UINode::GetTagName() const
{
	if (!_canvas)
		return String::EMPTY;

	const Vector<String>& tagNames = _canvas->TagNames();
	return _tag < tagNames.Size() ? tagNames[_layer] : String::EMPTY;
}

size_t UINode::NumPersistentChildren() const
{
	size_t ret = 0;

	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (!child->IsTemporary())
			++ret;
	}

	return ret;
}

void UINode::AllChildren(Vector<UINode*>& result) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		result.Push(child);
		child->AllChildren(result);
	}
}

UINode* UINode::FindChild(const String& childName, bool recursive) const
{
	return FindChild(childName.CString(), recursive);
}

UINode* UINode::FindChild(const char* childName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (child->_name == childName)
			return child;
		else if (recursive && child->_children.Size())
		{
			UINode* childResult = child->FindChild(childName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

UINode* UINode::FindChild(StringHash childType, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (child->GetType() == childType)
			return child;
		else if (recursive && child->_children.Size())
		{
			UINode* childResult = child->FindChild(childType, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

UINode* UINode::FindChild(StringHash childType, const String& childName, bool recursive) const
{
	return FindChild(childType, childName.CString(), recursive);
}

UINode* UINode::FindChild(StringHash childType, const char* childName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (child->GetType() == childType && child->_name == childName)
			return child;
		else if (recursive && child->_children.Size())
		{
			UINode* childResult = child->FindChild(childType, childName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

UINode* UINode::FindChildByLayer(unsigned layerMask, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (child->GetLayerMask() && layerMask)
			return child;
		else if (recursive && child->_children.Size())
		{
			UINode* childResult = child->FindChildByLayer(layerMask, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

UINode* UINode::FindChildByTag(unsigned char tag, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (child->_tag == tag)
			return child;
		else if (recursive && child->_children.Size())
		{
			UINode* childResult = child->FindChildByTag(tag, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

UINode* UINode::FindChildByTag(const String& tagName, bool recursive) const
{
	return FindChildByTag(tagName.CString(), recursive);
}

UINode* UINode::FindChildByTag(const char* tagName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (!String::Compare(child->GetTagName().CString(), tagName))
			return child;
		else if (recursive && child->_children.Size())
		{
			UINode* childResult = child->FindChildByTag(tagName, recursive);
			if (childResult)
				return childResult;
		}
	}

	return nullptr;
}

void UINode::FindChildren(Vector<UINode*>& result, StringHash childType, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (child->GetType() == childType)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildren(result, childType, recursive);
	}
}

void UINode::FindChildrenByLayer(Vector<UINode*>& result, unsigned layerMask, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (child->GetLayerMask() & layerMask)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildrenByLayer(result, layerMask, recursive);
	}
}

void UINode::FindChildrenByTag(Vector<UINode*>& result, unsigned char tag, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (child->_tag == tag)
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildrenByTag(result, tag, recursive);
	}
}

void UINode::FindChildrenByTag(Vector<UINode*>& result, const String& tagName, bool recursive) const
{
	FindChildrenByTag(result, tagName.CString(), recursive);
}

void UINode::FindChildrenByTag(Vector<UINode*>& result, const char* tagName, bool recursive) const
{
	for (auto it = _children.Begin(); it != _children.End(); ++it)
	{
		UINode* child = *it;
		if (!String::Compare(child->GetTagName().CString(), tagName))
			result.Push(child);
		if (recursive && child->_children.Size())
			child->FindChildrenByTag(result, tagName, recursive);
	}
}

void UINode::SetCanvas(Canvas* newCanvas)
{
	Canvas* oldCanvas = _canvas;
	_canvas = newCanvas;
	OnCanvasSet(_canvas, oldCanvas);
}

void UINode::SetId(unsigned newId)
{
	_id = newId;
}

void UINode::SkipHierarchy(Stream& source)
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

void UINode::OnParentSet(UINode*, UINode*)
{
}

void UINode::OnCanvasSet(Canvas*, Canvas*)
{
}

void UINode::OnSetEnabled(bool)
{
}

}