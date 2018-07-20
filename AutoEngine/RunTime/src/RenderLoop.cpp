#include "RenderLoop.h"
#include "RenderManager.h"
#include "BaseSpace.h"
AUTO_BEGIN

RenderLoop::RenderLoop(Camera& camera)
{
	_context.camera = &camera;
	_context.renderLoop = this;
	INSTANCE(RenderManager).AddCamera(&camera);
}

RenderLoop::~RenderLoop()
{
}

void RenderLoop::RunLoop()
{
	Camera * cam = _context.camera;
	RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
	GrViewPort(
		cam->GetViewRect().x * rect.width,
		cam->GetViewRect().y * rect.height,
		cam->GetViewRect().width * rect.width,
		cam->GetViewRect().height * rect.height
	);
	
	INSTANCE(BaseSpace).Draw();
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

