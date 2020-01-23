#include "Gameplay/World.h"
#include "Gameplay/WorldContext.h"

namespace Auto3D
{

AWorld::AWorld() 
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
}

}
