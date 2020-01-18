#include "Engine.h"
#include "Renderer/ForwardShadingRenderer.h"
#include "UI/UI.h"
#include "Platform/PlatformDef.h"
#include "Time/Time.h"
#include "Math/Random.h"

namespace Auto3D
{
	FForwardShadingRenderer* renderer;
/// Construct
FEngine::FEngine()
{
	renderer = new FForwardShadingRenderer();
}
/// Cestructor
FEngine::~FEngine()
{

}
/// Init engine, Register some SubSystem
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
/// AEngine normal exit
void FEngine::Exit()
{
	imguiDestroy();
	// Shutdown bgfx.
	bgfx::shutdown();
}
/// Render geometry
void FEngine::Render()
{

	renderer->Render();

	// Advance to next frame. Rendering thread will be kicked to
	// process submitted rendering primitives.
	bgfx::frame();
}
/// Sub system update data, If pause when _minimized -mode return false
bool FEngine::Update()
{

	return true;
}
/// Frame finish
void FEngine::FrameFinish()
{
	FTimeModule::Get().Update();
}

}