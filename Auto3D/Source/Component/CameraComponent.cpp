#include "Component/CameraComponent.h"
namespace Auto3D
{

ACameraComponent::ACameraComponent()
{

}
ACameraComponent::~ACameraComponent()
{

}

void ACameraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

}