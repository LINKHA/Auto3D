#include "Engine.h"
#include "Renderer/ForwardShadingRenderer.h"
#include "UI/UI.h"
#include "Platform/PlatformDef.h"
#include "Time/Time.h"
#include "Math/Random.h"
#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"

namespace Auto3D
{
FForwardShadingRenderer* renderer;

FEngine::FEngine()
{
	renderer = new FForwardShadingRenderer();
}

FEngine::~FEngine()
{

}

bool FEngine::Init()
{// Set random seeds based on time
	FTimeModule::RealTime& realTime = FTimeModule::Get().GetRealTime();

	SetRandomSeed(((unsigned)(realTime._day & 0xff) << 24) |
		((unsigned)(realTime._hour & 0xff) << 16) |
		((unsigned)(realTime._minute & 0xff) << 8) |
		((unsigned)(realTime._second & 0xff)));

	renderer->Init(AUTO_DEFAULT_WIDTH, AUTO_DEFAULT_HEIGHT);
	imguiCreate();

	return true;
}

void FEngine::Exit()
{
	imguiDestroy();
	// Shutdown bgfx.
	bgfx::shutdown();
}

void FEngine::Render()
{
	renderer->Render();
}

bool FEngine::Update()
{
	FTimeModule& time = FTimeModule::Get();
	// Begin is still dirty if you manually replace the world 
	auto world = FWorldContext::Get().GetActiveWorld();
	if (world && !world->HasBegunPlay())
		world->BeginPlay();

	time.Update();

	if (world && world->HasBegunPlay())
		world->Tick(time.GetDeltaTime());

	return true;
}

void FEngine::FrameFinish()
{
	
}

}