#include "Engine.h"
#include "Renderer/ForwardShadingRenderer.h"
#include "UI/UI.h"
#include "Platform/PlatformDef.h"
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
{
	renderer->Init(AUTO_DEFAULT_WIDTH, AUTO_DEFAULT_HEIGHT);

	return true;
}
/// AEngine normal exit
void FEngine::Exit()
{

}
/// Render geometry
void FEngine::Render()
{

}
/// Sub system update data,  If pause when _minimized -mode return false
bool FEngine::Update()
{

	return true;
}
/// Frame finish
void FEngine::FrameFinish()
{

}
}