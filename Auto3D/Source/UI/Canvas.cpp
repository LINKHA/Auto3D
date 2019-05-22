#include "../Debug/Log.h"
#include "../Debug/Profiler.h"
#include "../IO/Stream.h"
#include "../Object/ObjectResolver.h"
#include "../Resource/JSONFile.h"
#include "Canvas.h"

#include "../Scene/SpatialNode.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Canvas::Canvas() :
	_nextNodeId(1),
	_title(String::EMPTY),
	_closeButton(false),
	_close(false)
{
	// Register self to allow finding by ID
	AddNode(this);

	DefineLayer(LAYER_DEFAULT, "Default");
	DefineTag(TAG_NONE, "None");
}
Canvas::~Canvas()
{
	// Node destructor will also remove children. But at that point the node<>_id maps have been destroyed 
	// so must tear down the scene tree already here
	RemoveAllChildren();
	RemoveNode(this);
	assert(_nodes.IsEmpty());
}

void Canvas::RegisterObject()
{
	RegisterFactory<Canvas>();
	CopyBaseAttributes<Canvas, UINode>();
	RegisterAttribute("layerNames", &Canvas::LayerNamesAttr, &Canvas::SetLayerNamesAttr);
	RegisterAttribute("tagNames", &Canvas::TagNamesAttr, &Canvas::SetTagNamesAttr);
}


void Canvas::Save(Stream& dest)
{
	PROFILE(SaveScene);

	InfoString("Saving scene to " + dest.Name());

	dest.WriteFileID("SCNE");
	UINode::Save(dest);
}

bool Canvas::Load(Stream& source)
{
	PROFILE(LoadScene);

	InfoString("Loading scene from " + source.Name());

	String fileId = source.ReadFileID();
	if (fileId != "SCNE")
	{
		ErrorString("File is not a binary scene file");
		return false;
	}

	StringHash ownType = source.Read<StringHash>();
	unsigned ownId = source.Read<unsigned>();
	if (ownType != GetTypeStatic())
	{
		ErrorString("Mismatching type of scene root node in scene file");
		return false;
	}

	Clear();

	ObjectResolver resolver;
	resolver.StoreObject(ownId, this);
	UINode::Load(source, resolver);
	resolver.Resolve();

	return true;
}

bool Canvas::LoadJSON(const JSONValue& source)
{
	PROFILE(LoadSceneJSON);

	StringHash ownType(source["type"].GetString());
	unsigned ownId = (unsigned)source["id"].GetNumber();

	if (ownType != GetTypeStatic())
	{
		ErrorString("Mismatching type of scene root node in scene file");
		return false;
	}

	Clear();

	ObjectResolver resolver;
	resolver.StoreObject(ownId, this);
	UINode::LoadJSON(source, resolver);
	resolver.Resolve();

	return true;
}

bool Canvas::LoadJSON(Stream& source)
{
	InfoString("Loading scene from " + source.Name());

	JSONFile json;
	bool success = json.Load(source);
	LoadJSON(json.Root());
	return success;
}

bool Canvas::SaveJSON(Stream& dest)
{
	PROFILE(SaveCanvasJSON);

	InfoString("Saving scene to " + dest.Name());

	JSONFile json;
	UINode::SaveJSON(json.Root());
	return json.Save(dest);
}

UINode* Canvas::Instantiate(Stream& source)
{
	PROFILE(Instantiate);

	ObjectResolver resolver;
	StringHash childType(source.Read<StringHash>());
	unsigned childId = source.Read<unsigned>();

	UINode* child = CreateChild(childType);
	if (child)
	{
		resolver.StoreObject(childId, child);
		child->Load(source, resolver);
		resolver.Resolve();
	}

	return child;
}

UINode* Canvas::InstantiateJSON(const JSONValue& source)
{
	PROFILE(InstantiateJSON);

	ObjectResolver resolver;
	StringHash childType(source["type"].GetString());
	unsigned childId = (unsigned)source["id"].GetNumber();

	UINode* child = CreateChild(childType);
	if (child)
	{
		resolver.StoreObject(childId, child);
		child->LoadJSON(source, resolver);
		resolver.Resolve();
	}

	return child;
}

UINode* Canvas::InstantiateJSON(Stream& source)
{
	JSONFile json;
	json.Load(source);
	return InstantiateJSON(json.Root());
}

void Canvas::Clear()
{
	RemoveAllChildren();
	_nextNodeId = 1;
}

UINode* Canvas::FindNode(unsigned id) const
{
	auto it = _nodes.Find(id);
	return it != _nodes.End() ? it->_second : nullptr;
}

const HashMap<unsigned, UINode*>& Canvas::GetAllNode() const
{
	return _nodes;
}

void Canvas::AddNode(UINode* node)
{
	if (!node || node->ParentCanvas() == this)
		return;

	while (_nodes.Contains(_nextNodeId))
	{
		++_nextNodeId;
		if (!_nextNodeId)
			++_nextNodeId;
	}

	Canvas* oldScene = node->ParentCanvas();
	if (oldScene)
	{
		unsigned oldId = node->Id();
		oldScene->_nodes.Erase(oldId);
	}
	_nodes[_nextNodeId] = node;
	node->SetCanvas(this);
	node->SetId(_nextNodeId);

	++_nextNodeId;

	// If node has children, add them to the scene as well
	if (node->NumChildren())
	{
		const Vector<SharedPtr<UINode> >& children = node->Children();
		for (auto it = children.Begin(); it != children.End(); ++it)
			AddNode(*it);
	}
}

void Canvas::RemoveNode(UINode* node)
{
	if (!node || node->ParentCanvas() != this)
		return;

	_nodes.Erase(node->Id());
	node->SetCanvas(nullptr);
	node->SetId(0);

	// If node has children, remove them from the scene as well
	if (node->NumChildren())
	{
		const Vector<SharedPtr<UINode> >& children = node->Children();
		for (auto it = children.Begin(); it != children.End(); ++it)
			RemoveNode(*it);
	}
}

void Canvas::SetTitle(const String& title)
{
	_title = title;
}

void Canvas::SetTitle(const char* title)
{
	_title.Clear();
	_title += title;
}

void Canvas::SetLayerNamesAttr(JSONValue names)
{
	_layerNames.Clear();
	_layers.Clear();

	const JSONArray& array = names.GetArray();
	for (size_t i = 0; i < array.Size(); ++i)
	{
		const String& name = array[i].GetString();
		_layerNames.Push(name);
		_layers[name] = (unsigned char)i;
	}
}

JSONValue Canvas::LayerNamesAttr() const
{
	JSONValue ret;

	ret.SetEmptyArray();
	for (auto it = _layerNames.Begin(); it != _layerNames.End(); ++it)
		ret.Push(*it);

	return ret;
}

void Canvas::SetTagNamesAttr(JSONValue names)
{
	_tagNames.Clear();
	_tags.Clear();

	const JSONArray& array = names.GetArray();
	for (size_t i = 0; i < array.Size(); ++i)
	{
		const String& name = array[i].GetString();
		_tagNames.Push(name);
		_tags[name] = (unsigned char)i;
	}
}

JSONValue Canvas::TagNamesAttr() const
{
	JSONValue ret;

	ret.SetEmptyArray();
	for (auto it = _tagNames.Begin(); it != _tagNames.End(); ++it)
		ret.Push(*it);

	return ret;
}

}