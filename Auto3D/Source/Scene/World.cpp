#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "IO/Stream.h"
#include "Resource/JSONFile.h"
#include "Renderer/Renderer.h"
#include "Scene/WorldContext.h"
#include "Physics/PhysicsWorld.h"
#include "Engine/Components/SkyBox.h"
#include "Engine/Components/Camera.h"
#include "Serialization/Serialization.h"

#include "Scene/World.h"
#include "Transform.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

AWorld::AWorld() :
    _nextNodeId(1),
	_physicsWorld(nullptr),
	_skybox(nullptr),
	_shadowMapNum(0),
	_shadowMapSize(0)
{
    // Register self to allow finding by ID
    AddNode(this);

    DefineLayer(LAYER_DEFAULT, "Default");
    DefineTag(TAG_NONE, "None");

	// Register scene to scene system use to render
	GWorldContext::Get().RegisterWorld(this);
}

AWorld::~AWorld()
{
    // ANode destructor will also remove children. But at that point the node<>_id maps have been destroyed 
    // so must tear down the scene tree already here
    RemoveAllChildrenNode();
    RemoveNode(this);
    assert(_nodes.IsEmpty());
}

void AWorld::BeginPlay()
{
	Super::BeginPlay();
}

bool AWorld::Save(FStream& dest)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->SaveRoot(dest, this);
}

bool AWorld::Load(FStream& source)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->LoadRoot(source, this);
}

bool AWorld::LoadJSON(const FJSONValue& source)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->LoadRootJSON(source, this);
}

bool AWorld::LoadJSON(FStream& source)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->LoadRootJSON(source, this);
}

bool AWorld::SaveJSON(FStream& dest)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->SaveRootJSON(dest, this);
}

void AWorld::Clear()
{
    RemoveAllChildrenNode();
    _nextNodeId = 1;
}

ANode* AWorld::FindNode(unsigned id) const
{
    auto it = _nodes.Find(id);
    return it != _nodes.End() ? it->_second : nullptr;
}

TVector<ACamera*>& AWorld::GetCameras()
{
	return _cameras;
}

void AWorld::AddNode(ANode* node)
{
    if (!node || node->GetWorld() == this)
        return;

	if (node->GetTypeHash() == ACamera::GetTypeHashStatic())
	{
		AddCamera(dynamic_cast<ACamera*>(node));
	}

	if (node->GetTypeHash() == APhysicsWorld::GetTypeHashStatic())
	{
		SetPhysicsWorld(dynamic_cast<APhysicsWorld*>(node));
	}

    while (_nodes.Contains(_nextNodeId))
    {
        ++_nextNodeId;
        if (!_nextNodeId)
            ++_nextNodeId;
    }

    AWorld* oldScene = node->GetWorld();
    if (oldScene)
    {
        unsigned oldId = node->Id();
        oldScene->_nodes.Erase(oldId);
    }
    _nodes[_nextNodeId] = node;
    node->SetScene(this);
    node->SetId(_nextNodeId);

    ++_nextNodeId;

    // If node has children, add them to the scene as well
    if (node->NumChildren())
    {
        const TVector<TSharedPtr<ANode> >& children = node->GetChildrenNode();
        for (auto it = children.Begin(); it != children.End(); ++it)
            AddNode(*it);
    }
}

void AWorld::RemoveNode(ANode* node)
{
    if (!node || node->GetWorld() != this)
        return;

    _nodes.Erase(node->Id());
    node->SetScene(nullptr);
    node->SetId(0);
    
    // If node has children, remove them from the scene as well
    if (node->NumChildren())
    {
        const TVector<TSharedPtr<ANode> >& children = node->GetChildrenNode();
        for (auto it = children.Begin(); it != children.End(); ++it)
            RemoveNode(*it);
    }
}

void AWorld::AddCamera(ACamera* camera)
{
	if(camera)
		_cameras.Push(camera);
}

void AWorld::RemoveCamera(ACamera* camera)
{ 
	if (camera)
		_cameras.Remove(camera); 
}

TVector2F AWorld::GetShadowMapAttr()
{
	return TVector2F(_shadowMapNum, _shadowMapSize);
}

void AWorld::SetupShadowMapAttr(TVector2F numAndSize)
{
	SetupShadowMap(numAndSize._x, numAndSize._y);
}

void AWorld::SetPhysicsWorld(APhysicsWorld* physicsWorld)
{
	if(physicsWorld)
		_physicsWorld = physicsWorld;
}

void AWorld::SetSkyBox(ASkyBox* skybox)
{
	_skybox = skybox;
}

void AWorld::DefineLayer(unsigned char index, const FString& name)
{
	if (index >= 32)
	{
		ErrorString("Can not define more than 32 layers");
		return;
	}
	_defineLayers[name] = index;
}

void AWorld::DefineTag(unsigned char index, const FString& name)
{
	if (index >= 32)
	{
		ErrorString("Can not define more than 32 layers");
		return;
	}
	_defineTags[name] = index;
}

APhysicsWorld* AWorld::GetPhysicsWorld()
{
	if (_physicsWorld)
	{
		return _physicsWorld;
	}
	WarningString("Physics world failed to read, may be not created");
	return nullptr;
}

ASkyBox* AWorld::GetSkyBox()
{
	if (_skybox)
	{
		return _skybox;
	}
	WarningString("SkyBox failed to read, may be not created");
	return nullptr;
}

void AWorld::SetupShadowMap(size_t num, int size)
{
	_shadowMapNum = num;
	_shadowMapSize = size;
	// The scene creates a shadow map by default
	GModuleManager::Get().RendererModule()->SetupShadowMaps(num, size, EImageFormat::D16);
}

void AWorld::SetLayerNamesAttr(FJSONValue names)
{
    _defineLayers.Clear();

    const JSONArray& array = names.GetArray();
    for (size_t i = 0; i < array.Size(); ++i)
    {
        const FString& name = array[i].GetString();
		_defineLayers[name] = (unsigned char)i;
    }
}

FJSONValue AWorld::GetLayerNamesAttr() const
{
    FJSONValue ret;

    ret.SetEmptyArray();
    for (auto it = _defineLayers.Begin(); it != _defineLayers.End(); ++it)
        ret.Push(it->_first);
    
    return ret;
}

void AWorld::SetTagNamesAttr(FJSONValue names)
{
    _defineTags.Clear();

    const JSONArray& array = names.GetArray();
    for (size_t i = 0; i < array.Size(); ++i)
    {
        const FString& name = array[i].GetString();
        _defineTags[name] = (unsigned char)i;
    }
}

FJSONValue AWorld::GetTagNamesAttr() const
{
    FJSONValue ret;

    ret.SetEmptyArray();
    for (auto it = _defineTags.Begin(); it != _defineTags.End(); ++it)
		ret.Push(it->_first);

    return ret;
}

}
