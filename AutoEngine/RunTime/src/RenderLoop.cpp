#include "RenderLoop.h"
#include "RenderManager.h"
#include "MotionSpace.h"
AUTO_BEGIN

RenderLoop::RenderLoop(Camera& camera)
{
	m_Context.camera = &camera;
	m_Context.renderLoop = this;
	INSTANCE(RenderManager).AddCamera(&camera);
}

RenderLoop::~RenderLoop()
{
}
void RenderLoop::RunLoop()
{
	INSTANCE(MotionSpace).Update(m_Context.camera);
	INSTANCE(MotionSpace).Finish();
}
RenderLoop * CreateRenderLoop(Camera & camera)
{
	return new RenderLoop(camera);
}

void DeleteRenderLoop(RenderLoop * loop)
{
	delete loop;
}

void CleanUpAfterRenderLoop(RenderLoop & loop)
{
}

AUTO_END

