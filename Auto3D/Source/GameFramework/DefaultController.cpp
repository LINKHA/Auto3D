#include "DefaultController.h"
#include "GameFramework/Actor.h"
#include "Platform/Input.h"
#include "Core/Modules/ModuleManager.h"
#include "Graphics/Graphics.h"
#include "Renderer/Renderer.h"
#include "Time/Time.h"

namespace Auto3D
{
	
ADefaultController::ADefaultController() :
	yaw(0.0f),
	pitch(20.0f)
{

}
ADefaultController::~ADefaultController()
{

}

void ADefaultController::BeginPlay()
{
	Super::BeginPlay();
}

void ADefaultController::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);

	auto input = GModuleManager::Get().InputModule();
	auto graphics = GModuleManager::Get().GraphicsModule();
	auto renderer = GModuleManager::Get().RendererModule();
	auto time = GModuleManager::Get().TimeModule();

	AActor* attachedActor = GetAttachedActor();

	pitch += input->GetMouseMove()._y * 0.25f;
	yaw += input->GetMouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	attachedActor->SetRotation(FQuaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		attachedActor->Translate(TVector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		attachedActor->Translate(TVector3F::BACK * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		attachedActor->Translate(TVector3F::LEFT * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		attachedActor->Translate(TVector3F::RIGHT * time->GetDeltaTime() * moveSpeed);
}
}