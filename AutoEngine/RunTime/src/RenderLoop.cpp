#include "RenderLoop.h"

AUTO_BEGIN

RenderLoop::RenderLoop(Camera& camera)
{
	m_Context.m_Camera = &camera;
	m_Context.m_RenderLoop = this;
}

RenderLoop::~RenderLoop()
{
}

RenderLoop * CreateRenderLoop(Camera & camera)
{
	return new RenderLoop(camera);
}

void DeleteRenderLoop(RenderLoop * loop)
{
	delete loop;
}

void RunRenderLoop(RenderLoop & loop)
{
}

void CleanUpAfterRenderLoop(RenderLoop & loop)
{
}

AUTO_END

