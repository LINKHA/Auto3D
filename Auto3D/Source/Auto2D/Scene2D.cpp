#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "IO/Stream.h"
#include "Scene/Transform.h"
#include "Resource/JSONFile.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Core/Modules/ModuleManager.h"
#include "Auto2D/PhysicsWorld2D.h"
#include "Serialization/Serialization.h"
#include "Auto2D/Camera2D.h"
#include "Scene2D.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AScene2D)
	.constructor<>()
	.property_readonly("cameras", &AScene2D::GetCameras)
	.property("physicsWorld", &AScene2D::GetPhysicsWorld, &AScene2D::SetPhysicsWorld)
	.property("layerNames", &AScene2D::GetLayerNamesAttr, &AScene2D::SetLayerNamesAttr)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("tagNames", &AScene2D::GetTagNamesAttr, &AScene2D::SetTagNamesAttr)
	(
		metadata(SERIALIZABLE, "")
	)
	;
}

AScene2D::AScene2D() :
	_nextNodeId(1)
{
	// Register self to allow finding by ID
	AddNode(this);

	DefineLayer(LAYER_DEFAULT, "Default");
	DefineTag(TAG_NONE, "None");
	GModuleManager::Get().RegisteredBoxModule()->RegisterScene2D(this);
}
AScene2D::~AScene2D()
{
	// ANode destructor will also remove children. But at that point the node<>_id maps have been destroyed 
   // so must tear down the scene tree already here
	RemoveAllChildren();
	RemoveNode(this);
	assert(_nodes.IsEmpty());
}

bool AScene2D::Save(FStream& dest)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->SaveRoot(dest, this);
}

bool AScene2D::Load(FStream& source)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->LoadRoot(source, this);
}

bool AScene2D::LoadJSON(const FJSONValue& source)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->LoadRootJSON(source, this);
}

bool AScene2D::LoadJSON(FStream& source)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->LoadRootJSON(source, this);
}

bool AScene2D::SaveJSON(FStream& dest)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->SaveRootJSON(dest, this);
}

void AScene2D::Clear()
{
	RemoveAllChildren();
	_nextNodeId = 1;
}

ANode2D* AScene2D::FindNode(unsigned id) const
{
	auto it = _nodes.Find(id);
	return it != _nodes.End() ? it->_second : nullptr;
}

const THashMap<unsigned, ANode2D*>& AScene2D::GetAllNode() const
{
	return _nodes;
}

TVector<ACamera2D*>& AScene2D::GetCameras()
{
	return _cameras;
}

void AScene2D::AddNode(ANode2D* node)
{
	if (!node || node->ParentScene2D() == this)
		return;

	if (node->GetTypeHash() == ACamera2D::GetTypeHashStatic())
	{
		AddCamera(dynamic_cast<ACamera2D*>(node));
	}

	if (node->GetTypeHash() == APhysicsWorld2D::GetTypeHashStatic())
	{
		SetPhysicsWorld(dynamic_cast<APhysicsWorld2D*>(node));
	}

	while (_nodes.Contains(_nextNodeId))
	{
		++_nextNodeId;
		if (!_nextNodeId)
			++_nextNodeId;
	}

	AScene2D* oldScene = node->ParentScene2D();
	if (oldScene)
	{
		unsigned oldId = node->Id();
		oldScene->_nodes.Erase(oldId);
	}
	_nodes[_nextNodeId] = node;
	node->SetScene2D(this);
	node->SetId(_nextNodeId);

	++_nextNodeId;

	// If node has children, add them to the scene as well
	if (node->NumChildren())
	{
		const TVector<TSharedPtr<ANode2D> >& children = node->Children();
		for (auto it = children.Begin(); it != children.End(); ++it)
			AddNode(*it);
	}
}

void AScene2D::RemoveNode(ANode2D* node)
{
	if (!node || node->ParentScene2D() != this)
		return;

	_nodes.Erase(node->Id());
	node->SetScene2D(nullptr);
	node->SetId(0);

	// If node has children, remove them from the scene as well
	if (node->NumChildren())
	{
		const TVector<TSharedPtr<ANode2D> >& children = node->Children();
		for (auto it = children.Begin(); it != children.End(); ++it)
			RemoveNode(*it);
	}
}

void AScene2D::SetPhysicsWorld(APhysicsWorld2D* physicsWorld)
{
	_physicsWorld = physicsWorld;
}

APhysicsWorld2D* AScene2D::GetPhysicsWorld()
{
	if (_physicsWorld)
	{
		return _physicsWorld;
	}
	WarningString("Physics world failed to read, may be not created");
	return nullptr;
}

void AScene2D::DefineLayer(unsigned char index, const FString& name)
{
	if (index >= 32)
	{
		ErrorString("Can not define more than 32 layers");
		return;
	}
	_defineLayers[name] = index;
}

void AScene2D::DefineTag(unsigned char index, const FString& name)
{
	if (index >= 32)
	{
		ErrorString("Can not define more than 32 layers");
		return;
	}
	_defineTags[name] = index;
}

void AScene2D::SetLayerNamesAttr(FJSONValue names)
{
	_defineLayers.Clear();

	const JSONArray& array = names.GetArray();
	for (size_t i = 0; i < array.Size(); ++i)
	{
		const FString& name = array[i].GetString();
		_defineLayers[name] = (unsigned char)i;
	}
}

FJSONValue AScene2D::GetLayerNamesAttr() const
{
	FJSONValue ret;

	ret.SetEmptyArray();
	for (auto it = _defineLayers.Begin(); it != _defineLayers.End(); ++it)
		ret.Push(it->_first);

	return ret;
}

void AScene2D::SetTagNamesAttr(FJSONValue names)
{
	_defineTags.Clear();

	const JSONArray& array = names.GetArray();
	for (size_t i = 0; i < array.Size(); ++i)
	{
		const FString& name = array[i].GetString();
		_defineTags[name] = (unsigned char)i;
	}
}

FJSONValue AScene2D::GetTagNamesAttr() const
{
	FJSONValue ret;

	ret.SetEmptyArray();
	for (auto it = _defineTags.Begin(); it != _defineTags.End(); ++it)
		ret.Push(it->_first);

	return ret;
}

}