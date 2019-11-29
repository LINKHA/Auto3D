#include "Debug/Log.h"
#include "IO/Stream.h"
#include "Core/ObjectResolver.h"
#include "Resource/JSONFile.h"
#include "Engine/Components/Camera.h"
#include "Scene.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

static Vector<SharedPtr<Node> > noChildren;

Node::Node() :
    _flags(NF_ENABLED),
    _layer(LAYER_DEFAULT),
    _tag(TAG_NONE),
    _parent(nullptr),
    _scenes(nullptr),
    _id(0)
{
}

Node::~Node()
{
    RemoveAllChildren();
    // At the time of destruction the node should not have a parent, or be in a scene
    assert(!_parent);
    assert(!_scenes);
}

void Node::RegisterObject()
{
    RegisterFactory<Node>();
    RegisterRefAttribute("name", &Node::GetName, &Node::SetName);
    RegisterAttribute("enabled", &Node::IsEnabled, &Node::SetEnabled, true);
    RegisterAttribute("temporary", &Node::IsTemporary, &Node::SetTemporary, false);
    RegisterAttribute("layer", &Node::GetLayer, &Node::SetLayer, LAYER_DEFAULT);
    RegisterAttribute("tag", &Node::GetTag, &Node::SetTag, TAG_NONE);
}

void Node::Load(Stream& source, ObjectResolver& resolver)
{
    // Type and _id has been read by the parent
    Serializable::Load(source, resolver);

    size_t numChildren = source.ReadVLE();
    for (size_t i = 0; i < numChildren; ++i)
    {
        StringHash childType(source.Read<StringHash>());
        unsigned childId = source.Read<unsigned>();
        Node* child = CreateChild(childType);
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

void Node::Save(Stream& dest)
{
    // Write type and ID first, followed by attributes and child nodes
    dest.Write(GetType());
    dest.Write(Id());
    Serializable::Save(dest);
    dest.WriteVLE(NumPersistentChildren());

    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (!child->IsTemporary())
            child->Save(dest);
    }
}

void Node::LoadJSON(const JSONValue& source, ObjectResolver& resolver)
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
            Node* child = CreateChild(childType);
            if (child)
            {
                resolver.StoreObject(childId, child);
                child->LoadJSON(childJSON, resolver);
            }
        }
    }
}

void Node::SaveJSON(JSONValue& dest)
{
    dest["type"] = GetTypeName();
    dest["id"] = Id();
    Serializable::SaveJSON(dest);
    
    if (NumPersistentChildren())
    {
        dest["children"].SetEmptyArray();
        for (auto it = _children.Begin(); it != _children.End(); ++it)
        {
            Node* child = *it;
            if (!child->IsTemporary())
            {
                JSONValue childJSON;
                child->SaveJSON(childJSON);
                dest["children"].Push(childJSON);
            }
        }
    }
}

bool Node::SaveJSON(Stream& dest)
{
    JSONFile json;
    SaveJSON(json.Root());
    return json.Save(dest);
}

void Node::SetName(const String& newName)
{
    SetName(newName.CString());
}

void Node::SetName(const char* newName)
{
    _name = newName;
}

void Node::SetLayer(unsigned char newLayer)
{
    if (_layer < 32)
        _layer = newLayer;
    else
        ErrorString("Can not set layer 32 or higher");
}

void Node::SetLayerName(const String& newLayerName)
{
    if (!_scenes)
        return;
    
    const HashMap<String, unsigned char>& layers = _scenes->Layers();
    auto it = layers.Find(newLayerName);
    if (it != layers.End())
		SetLayer(it->_second);
    else
        ErrorString("Layer " + newLayerName + " not defined in the scene");
}

void Node::SetTag(unsigned char newTag)
{
    _tag = newTag;
}

void Node::SetTagName(const String& newTagName)
{
    if (!_scenes)
        return;

    const HashMap<String, unsigned char>& tags = _scenes->Tags();
    auto it = tags.Find(newTagName);
    if (it != tags.End())
        SetTag(it->_second);
    else
        ErrorString("Tag " + newTagName + " not defined in the scene");
}

void Node::SetEnabled(bool enable)
{
    SetFlag(NF_ENABLED, enable);
    OnSetEnabled(TestFlag(NF_ENABLED));
}

void Node::SetEnabledRecursive(bool enable)
{
    SetEnabled(enable);
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        child->SetEnabledRecursive(enable);
    }
}

void Node::SetTemporary(bool enable)
{
    SetFlag(NF_TEMPORARY, enable);
}

void Node::SetParent(Node* newParent)
{
    if (newParent)
        newParent->AddChild(this);
    else
        ErrorString("Could not set null parent");
}

void Node::DefineLayer(unsigned char index, const String& name)
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

void Node::DefineTag(unsigned char index, const String& name)
{
	if (_tagNames.Size() <= index)
		_tagNames.Resize(index + 1);
	_tagNames[index] = name;
	_tags[name] = index;
}

Node* Node::CreateChild(StringHash childType)
{
    SharedPtr<AObject> newObject = Create(childType);
    if (!newObject)
    {
        ErrorString("Could not create child node of unknown type " + childType.ToString());
        return nullptr;
    }
    Node* child = dynamic_cast<Node*>(newObject.Get());
    if (!child)
    {
        ErrorString(newObject->GetTypeName() + " is not a Node subclass, could not add as a child");
        return nullptr;
    }

    AddChild(child);
	child->ParentCallBack();

    return child;
}

Node* Node::CreateChild(StringHash childType, const String& childName)
{
    return CreateChild(childType, childName.CString());
}

Node* Node::CreateChild(StringHash childType, const char* childName)
{
    Node* child = CreateChild(childType);
    if (child)
        child->SetName(childName);
    return child;
}

void Node::AddChild(Node* child)
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
    Node* current = _parent;
    while (current)
    {
        if (current == child)
        {
            ErrorString("Attempted cyclic node parenting");
            return;
        }
        current = current->_parent;
    }

    Node* oldParent = child->_parent;
    if (oldParent)
        oldParent->_children.Remove(child);
    _children.Push(child);
    child->_parent = this;
    child->OnParentSet(this, oldParent);
	if (_scenes)
	{
		_scenes->AddNode(child);
		if (child->GetType() == Camera::GetTypeStatic())
		{
			_scenes->AddCamera(dynamic_cast<Camera*>(child));
		}
	}
        
}

void Node::RemoveChild(Node* child)
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

void Node::RemoveChild(size_t index)
{
    if (index >= _children.Size())
        return;

    Node* child = _children[index];
    // Detach from both the parent and the scene (removes _id assignment)
    child->_parent = nullptr;
    child->OnParentSet(this, nullptr);
    if (_scenes)
        _scenes->RemoveNode(child);
    _children.Erase(index);
}

void Node::RemoveAllChildren()
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        child->_parent = nullptr;
        child->OnParentSet(this, nullptr);
        if (_scenes)
            _scenes->RemoveNode(child);
        it->Reset();
    }

    _children.Clear();
}

void Node::RemoveSelf()
{
    if (_parent)
        _parent->RemoveChild(this);
    else
        delete this;
}

const String& Node::GetLayerName() const
{
    if (!_scenes)
        return String::EMPTY;

    const Vector<String>& layerNames = _scenes->LayerNames();
    return _layer < layerNames.Size() ? layerNames[_layer] : String::EMPTY;
}

const String& Node::GetTagName() const
{
    if (!_scenes)
        return String::EMPTY;

    const Vector<String>& tagNames = _scenes->TagNames();
    return _tag < tagNames.Size() ? tagNames[_layer] : String::EMPTY;
}

size_t Node::NumPersistentChildren() const
{
    size_t ret = 0;

    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (!child->IsTemporary())
            ++ret;
    }

    return ret;
}

void Node::AllChildren(Vector<Node*>& result) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        result.Push(child);
        child->AllChildren(result);
    }
}

Node* Node::FindChild(const String& childName, bool recursive) const
{
    return FindChild(childName.CString(), recursive);
}

Node* Node::FindChild(const char* childName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (child->_name == childName)
            return child;
        else if (recursive && child->_children.Size())
        {
            Node* childResult = child->FindChild(childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

Node* Node::FindChild(StringHash childType, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (child->GetType() == childType)
            return child;
        else if (recursive && child->_children.Size())
        {
            Node* childResult = child->FindChild(childType, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

Node* Node::FindChild(StringHash childType, const String& childName, bool recursive) const
{
    return FindChild(childType, childName.CString(), recursive);
}

Node* Node::FindChild(StringHash childType, const char* childName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (child->GetType() == childType && child->_name == childName)
            return child;
        else if (recursive && child->_children.Size())
        {
            Node* childResult = child->FindChild(childType, childName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

Node* Node::FindChildByLayer(unsigned layerMask, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (child->GetLayerMask() && layerMask)
            return child;
        else if (recursive && child->_children.Size())
        {
            Node* childResult = child->FindChildByLayer(layerMask, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

Node* Node::FindChildByTag(unsigned char tag, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (child->_tag == tag)
            return child;
        else if (recursive && child->_children.Size())
        {
            Node* childResult = child->FindChildByTag(tag, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

Node* Node::FindChildByTag(const String& tagName, bool recursive) const
{
    return FindChildByTag(tagName.CString(), recursive);
}

Node* Node::FindChildByTag(const char* tagName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (!String::Compare(child->GetTagName().CString(), tagName))
            return child;
        else if (recursive && child->_children.Size())
        {
            Node* childResult = child->FindChildByTag(tagName, recursive);
            if (childResult)
                return childResult;
        }
    }

    return nullptr;
}

void Node::FindChildren(Vector<Node*>& result, StringHash childType, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (child->GetType() == childType)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildren(result, childType, recursive);
    }
}

void Node::FindChildrenByLayer(Vector<Node*>& result, unsigned layerMask, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (child->GetLayerMask() & layerMask)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByLayer(result, layerMask, recursive);
    }
}

void Node::FindChildrenByTag(Vector<Node*>& result, unsigned char tag, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (child->_tag == tag)
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByTag(result, tag, recursive);
    }
}

void Node::FindChildrenByTag(Vector<Node*>& result, const String& tagName, bool recursive) const
{
    FindChildrenByTag(result, tagName.CString(), recursive);
}

void Node::FindChildrenByTag(Vector<Node*>& result, const char* tagName, bool recursive) const
{
    for (auto it = _children.Begin(); it != _children.End(); ++it)
    {
        Node* child = *it;
        if (!String::Compare(child->GetTagName().CString(), tagName))
            result.Push(child);
        if (recursive && child->_children.Size())
            child->FindChildrenByTag(result, tagName, recursive);
    }
}

void Node::SetScene(Scene* newScene)
{
    Scene* oldScene = _scenes;
    _scenes = newScene;
    OnSceneSet(_scenes, oldScene);
}

void Node::SetId(unsigned newId)
{
    _id = newId;
}

void Node::SkipHierarchy(Stream& source)
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

void Node::OnParentSet(Node*, Node*)
{
}

void Node::OnSceneSet(Scene*, Scene*)
{
}

void Node::OnSetEnabled(bool)
{
}

}
