#include "Engine.h"
#include "Renderer/ForwardShadingRenderer.h"
#include "UI/UI.h"
#include "Platform/PlatformDef.h"
#include "Time/Time.h"
#include "Math/Random.h"

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

	return true;
}

void FEngine::FrameFinish()
{
	FTimeModule::Get().Update();
}

}