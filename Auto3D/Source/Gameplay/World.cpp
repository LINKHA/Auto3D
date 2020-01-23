#include "Gameplay/World.h"
#include "Gameplay/WorldContext.h"


namespace Auto3D
{

AWorld::AWorld():
	_nextNodeId(1)
{
  
}

AWorld::~AWorld()
{

}

void AWorld::BeginPlay()
{
	Super::BeginPlay();
}

void AWorld::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void AWorld::OnRegister()
{
	FWorldContext::Get().RegisterWorld(SPtrThis());
	AddActor(SPtrThis());
}

void AWorld::AddActor(SPtr<AActor> actor)
{
	if (!actor || actor->GetWorld() == SPtrThis())
		return;

	while (_actors.Contains(_nextNodeId))
	{
		++_nextNodeId;
		if (!_nextNodeId)
			++_nextNodeId;
	}

	SPtr<AWorld> oldScene = actor->GetWorld();
	if (oldScene)
	{
		unsigned oldId = actor->Id();
		oldScene->GetActors().Erase(oldId);
	}
	_actors[_nextNodeId] = actor;
	actor->SetWorld(SPtrThis());
	actor->SetId(_nextNodeId);

	++_nextNodeId;

	// If node has children, add them to the scene as well
	if (actor->NumChildren())
	{
		TVector<SPtr<AActor>> children;
		actor->GetAllChildrenNode(children);

		for (auto it = children.Begin(); it != children.End(); ++it)
			AddActor(*it);
	}
}

void AWorld::RemoveActor(SPtr<AActor> actor)
{
	if (!actor || actor->GetWorld() == SPtrThis())
		return;

	_actors.Erase(actor->Id());
	actor->SetWorld(nullptr);
	actor->SetId(0);

	// If node has children, remove them from the scene as well
	if (actor->NumChildren())
	{
		TVector<SPtr<AActor>> children;
		actor->GetAllChildrenNode(children);
		for (auto it = children.Begin(); it != children.End(); ++it)
			RemoveActor(*it);
	}
}

void AWorld::AddCamera(SPtr<ACameraComponent> camera)
{
	if (camera)
		_cameras.Push(camera);
}

void AWorld::RemoveCamera(SPtr<ACameraComponent> camera)
{
	if (camera)
		_cameras.Remove(camera);
}

}
