#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "IO/Stream.h"
#include "Core/ObjectResolver.h"
#include "Resource/JSONFile.h"
#include "Renderer/Renderer.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Physics/PhysicsWorld.h"
#include "Engine/Components/SkyBox.h"
#include "Engine/Components/Camera.h"
#include "Serialization/Serialization.h"

#include "Scene.h"
#include "SpatialNode.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

REGISTER_CLASS
{
	using namespace rttr;
	registration::class_<AScene>("AScene")
	.constructor<>()
	.property_readonly("cameras", &AScene::GetCameras)
	.property("physicsWorld", &AScene::GetPhysicsWorld, &AScene::SetPhysicsWorld)
	.property("skybox", &AScene::GetSkyBox, &AScene::SetSkyBox)
	.property("shadowMapAttr",&AScene::GetShadowMapAttr,&AScene::SetupShadowMapAttr)
	(
		metadata(SERIALIZABLE,"")
	)
	.property("layerNames", &AScene::GetLayerNamesAttr, &AScene::SetLayerNamesAttr)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("tagNames", &AScene::GetTagNamesAttr, &AScene::SetTagNamesAttr)
	(
		metadata(SERIALIZABLE, "")
	)
	;
}

AScene::AScene() :
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
	GModuleManager::Get().RegisteredBoxModule()->RegisterScene(this);
}

AScene::~AScene()
{
    // ANode destructor will also remove children. But at that point the node<>_id maps have been destroyed 
    // so must tear down the scene tree already here
    RemoveAllChildren();
    RemoveNode(this);
    assert(_nodes.IsEmpty());
}

void AScene::RegisterObject()
{
    RegisterFactory<AScene>();
}

bool AScene::Save(FStream& dest)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->SaveRoot(dest, this);
}

bool AScene::Load(FStream& source)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->LoadRoot(source, this);
}

bool AScene::LoadJSON(const FJSONValue& source)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->LoadRootJSON(source, this);
}

bool AScene::LoadJSON(FStream& source)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->LoadRootJSON(source, this);
}

bool AScene::SaveJSON(FStream& dest)
{
	auto* serialization = GModuleManager::Get().SerializationModule();
	return serialization->SaveRootJSON(dest, this);
}

void AScene::Clear()
{
    RemoveAllChildren();
    _nextNodeId = 1;
}

ANode* AScene::FindNode(unsigned id) const
{
    auto it = _nodes.Find(id);
    return it != _nodes.End() ? it->_second : nullptr;
}

TVector<ACamera*>& AScene::GetCameras()
{
	return _cameras;
}

void AScene::AddNode(ANode* node)
{
    if (!node || node->ParentScene() == this)
        return;

    while (_nodes.Contains(_nextNodeId))
    {
        ++_nextNodeId;
        if (!_nextNodeId)
            ++_nextNodeId;
    }

    AScene* oldScene = node->ParentScene();
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
        const TVector<TSharedPtr<ANode> >& children = node->Children();
        for (auto it = children.Begin(); it != children.End(); ++it)
            AddNode(*it);
    }
}

void AScene::RemoveNode(ANode* node)
{
    if (!node || node->ParentScene() != this)
        return;

    _nodes.Erase(node->Id());
    node->SetScene(nullptr);
    node->SetId(0);
    
    // If node has children, remove them from the scene as well
    if (node->NumChildren())
    {
        const TVector<TSharedPtr<ANode> >& children = node->Children();
        for (auto it = children.Begin(); it != children.End(); ++it)
            RemoveNode(*it);
    }
}

void AScene::AddCamera(ACamera* camera)
{
	if(camera)
		_cameras.Push(camera);
}

void AScene::RemoveCamera(ACamera* camera)
{ 
	if (camera)
		_cameras.Remove(camera); 
}

TVector2F AScene::GetShadowMapAttr()
{
	return TVector2F(_shadowMapNum, _shadowMapSize);
}

void AScene::SetupShadowMapAttr(TVector2F numAndSize)
{
	SetupShadowMap(numAndSize._x, numAndSize._y);
}

void AScene::SetPhysicsWorld(APhysicsWorld* physicsWorld)
{
	if(physicsWorld)
		_physicsWorld = physicsWorld;
}

void AScene::SetSkyBox(ASkyBox* skybox)
{
	_skybox = skybox;
}

void AScene::DefineLayer(unsigned char index, const FString& name)
{
	if (index >= 32)
	{
		ErrorString("Can not define more than 32 layers");
		return;
	}
	_defineLayers[name] = index;
}

void AScene::DefineTag(unsigned char index, const FString& name)
{
	if (index >= 32)
	{
		ErrorString("Can not define more than 32 layers");
		return;
	}
	_defineTags[name] = index;
}

APhysicsWorld* AScene::GetPhysicsWorld()
{
	if (_physicsWorld)
	{
		return _physicsWorld;
	}
	WarningString("Physics world failed to read, may be not created");
	return nullptr;
}

ASkyBox* AScene::GetSkyBox()
{
	if (_skybox)
	{
		return _skybox;
	}
	WarningString("SkyBox failed to read, may be not created");
	return nullptr;
}

void AScene::SetupShadowMap(size_t num, int size)
{
	_shadowMapNum = num;
	_shadowMapSize = size;
	// The scene creates a shadow map by default
	GModuleManager::Get().RendererModule()->SetupShadowMaps(num, size, EImageFormat::D16);
}

void AScene::SetLayerNamesAttr(FJSONValue names)
{
    _defineLayers.Clear();

    const JSONArray& array = names.GetArray();
    for (size_t i = 0; i < array.Size(); ++i)
    {
        const FString& name = array[i].GetString();
		_defineLayers[name] = (unsigned char)i;
    }
}

FJSONValue AScene::GetLayerNamesAttr() const
{
    FJSONValue ret;

    ret.SetEmptyArray();
    for (auto it = _defineLayers.Begin(); it != _defineLayers.End(); ++it)
        ret.Push(it->_first);
    
    return ret;
}

void AScene::SetTagNamesAttr(FJSONValue names)
{
    _defineTags.Clear();

    const JSONArray& array = names.GetArray();
    for (size_t i = 0; i < array.Size(); ++i)
    {
        const FString& name = array[i].GetString();
        _defineTags[name] = (unsigned char)i;
    }
}

FJSONValue AScene::GetTagNamesAttr() const
{
    FJSONValue ret;

    ret.SetEmptyArray();
    for (auto it = _defineTags.Begin(); it != _defineTags.End(); ++it)
		ret.Push(it->_first);

    return ret;
}

void RegisterSceneLibrary()
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    ANode::RegisterObject();
    AScene::RegisterObject();
    ASpatialNode::RegisterObject();
}

}
