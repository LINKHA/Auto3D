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
    CopyBaseAttributes<AScene, ANode>();
    RegisterAttribute("layerNames", &AScene::LayerNamesAttr, &AScene::SetLayerNamesAttr);
    RegisterAttribute("tagNames", &AScene::TagNamesAttr, &AScene::SetTagNamesAttr);
}

void AScene::Save(FStream& dest)
{
    PROFILE(SaveScene);
    
    InfoString("Saving scene to " + dest.GetName());
    
    dest.WriteFileID("SCNE");
    ANode::Save(dest);
}

bool AScene::Load(FStream& source)
{
	PROFILE(LoadScene);

	InfoString("Loading scene from " + source.GetName());

	FString fileId = source.ReadFileID();
	if (fileId != "SCNE")
	{
		ErrorString("File is not a binary scene file");
		return false;
	}

	FStringHash ownType = source.Read<FStringHash>();
	unsigned ownId = source.Read<unsigned>();
	if (ownType != GetTypeStatic())
	{
		ErrorString("Mismatching type of scene root node in scene file");
		return false;
	}

	Clear();

	FObjectResolver resolver;
	resolver.StoreObject(ownId, this);
	ANode::Load(source, resolver);
	resolver.Resolve();

	return true;
}

bool AScene::LoadJSON(const FJSONValue& source)
{
    PROFILE(LoadSceneJSON);
    
    FStringHash ownType(source["type"].GetString());
    unsigned ownId = (unsigned)source["id"].GetNumber();

    if (ownType != GetTypeStatic())
    {
        ErrorString("Mismatching type of scene root node in scene file");
        return false;
    }

    Clear();

    FObjectResolver resolver;
    resolver.StoreObject(ownId, this);
    ANode::LoadJSON(source, resolver);
    resolver.Resolve();

    return true;
}

bool AScene::LoadJSON(FStream& source)
{
    InfoString("Loading scene from " + source.GetName());
    
    AJSONFile json;
    bool success = json.Load(source);
    LoadJSON(json.Root());
    return success;
}

bool AScene::SaveJSON(FStream& dest)
{
    PROFILE(SaveSceneJSON);
    
    InfoString("Saving scene to " + dest.GetName());
    
    AJSONFile json;
    ANode::SaveJSON(json.Root());
    return json.Save(dest);
}

ANode* AScene::Instantiate(FStream& source)
{
    PROFILE(Instantiate);
    
    FObjectResolver resolver;
    FStringHash childType(source.Read<FStringHash>());
    unsigned childId = source.Read<unsigned>();

    ANode* child = CreateChild(childType);
    if (child)
    {
        resolver.StoreObject(childId, child);
        child->Load(source, resolver);
        resolver.Resolve();
    }

    return child;
}

ANode* AScene::InstantiateJSON(const FJSONValue& source)
{
    PROFILE(InstantiateJSON);
    
    FObjectResolver resolver;
    FStringHash childType(source["type"].GetString());
    unsigned childId = (unsigned)source["id"].GetNumber();

    ANode* child = CreateChild(childType);
    if (child)
    {
        resolver.StoreObject(childId, child);
        child->LoadJSON(source, resolver);
        resolver.Resolve();
    }

    return child;
}

ANode* AScene::InstantiateJSON(FStream& source)
{
    AJSONFile json;
    json.Load(source);
    return InstantiateJSON(json.Root());
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
    _layerNames.Clear();
    _layers.Clear();

    const JSONArray& array = names.GetArray();
    for (size_t i = 0; i < array.Size(); ++i)
    {
        const FString& name = array[i].GetString();
        _layerNames.Push(name);
        _layers[name] = (unsigned char)i;
    }
}

FJSONValue AScene::LayerNamesAttr() const
{
    FJSONValue ret;

    ret.SetEmptyArray();
    for (auto it = _layerNames.Begin(); it != _layerNames.End(); ++it)
        ret.Push(*it);
    
    return ret;
}

void AScene::SetTagNamesAttr(FJSONValue names)
{
    _tagNames.Clear();
    _tags.Clear();

    const JSONArray& array = names.GetArray();
    for (size_t i = 0; i < array.Size(); ++i)
    {
        const FString& name = array[i].GetString();
        _tagNames.Push(name);
        _tags[name] = (unsigned char)i;
    }
}

FJSONValue AScene::TagNamesAttr() const
{
    FJSONValue ret;

    ret.SetEmptyArray();
    for (auto it = _tagNames.Begin(); it != _tagNames.End(); ++it)
        ret.Push(*it);

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
