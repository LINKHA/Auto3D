#include "Gameplay/World.h"
#include "Gameplay/WorldContext.h"
#include "Serialization/Serialization.h"
#include "Physics/PhysicsWorld.h"

namespace Auto3D
{

AWorld::AWorld():
	_nextNodeId(1)
{
	AddActor(this);
}

AWorld::~AWorld()
{
	_world = nullptr;
	_parent = nullptr;
}

void AWorld::BeginPlay()
{
	Super::BeginPlay();
}

void AWorld::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void AWorld::AddActor(AActor* actor)
{
	if (!actor || actor->GetWorld() == this)
		return;

	while (_actors.Contains(_nextNodeId))
	{
		++_nextNodeId;
		if (!_nextNodeId)
			++_nextNodeId;
	}

	AWorld* oldScene = actor->GetWorld();
	if (oldScene)
	{
		unsigned oldId = actor->GetActorID();
		oldScene->GetActors().Erase(oldId);
	}
	_actors[_nextNodeId] = actor;
	actor->SetWorld(this);
	actor->SetId(_nextNodeId);

	++_nextNodeId;

	// If node has children, add them to the scene as well
	if (actor->NumChildren())
	{
		TVector<AActor*> children;
		actor->GetAllChildren(children);

		for (auto it = children.Begin(); it != children.End(); ++it)
			AddActor(*it);
	}
}

void AWorld::RemoveActor(AActor* actor)
{
	if (!actor || actor->GetWorld() == this)
		return;

	_actors.Erase(actor->GetActorID());
	actor->SetWorld(nullptr);
	actor->SetId(0);

	// If node has children, remove them from the scene as well
	if (actor->NumChildren())
	{
		TVector<AActor*> children;
		actor->GetAllChildren(children);
		for (auto it = children.Begin(); it != children.End(); ++it)
			RemoveActor(*it);
	}
}

void AWorld::AddCamera(ACameraComponent* camera)
{
	if (camera)
		_cameras.Push(camera);
}

void AWorld::RemoveCamera(ACameraComponent* camera)
{
	if (camera)
		_cameras.Remove(camera);
}

bool AWorld::SaveJson(FStream& dest)
{
	GSerializationModule& serialization = GSerializationModule::Get();
	return serialization.SaveRootJSON(dest, this);
}

bool AWorld::LoadJson(const FString& path)
{
	return true;
}

FPhysicsWorld* AWorld::GetPhysicsWorld()
{
	if (!_physicsWorld)
	{
		_physicsWorld = new FPhysicsWorld;
	}
	return _physicsWorld;
}

void AWorld::Clear()
{
	RemoveAllChildren();
	_nextNodeId = 1;
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
