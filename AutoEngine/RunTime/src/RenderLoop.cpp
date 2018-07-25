#include "RenderLoop.h"
#include "RenderManager.h"
#include "BaseSpace.h"
#include "GLWindow.h"
#include "BaseSpace.h"
AUTO_BEGIN

RenderLoop::RenderLoop(Ambient* ambient,Camera& camera)
	:Super(ambient)
{
	_camera = &camera;
	INSTANCE(RenderManager).AddCamera(&camera);
}

RenderLoop::~RenderLoop()
{
}

void RenderLoop::RunLoop()
{
	//Camera * cam = _camera;
	RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
	glViewport(
		_camera->GetViewRect().x * rect.width,
		_camera->GetViewRect().y * rect.height,
		_camera->GetViewRect().width * rect.width,
		_camera->GetViewRect().height * rect.height
	);
	//INSTANCE(BaseSpace).Draw();
	GetSubSystem<BaseSpace>()->Draw();
}
RenderLoop * CreateRenderLoop(Ambient* ambient,Camera & camera)
{
	return new RenderLoop(ambient,camera);
}

void DeleteRenderLoop(RenderLoop * loop)
{
	delete loop;
}

void CleanUpAfterRenderLoop(RenderLoop & loop)
{
}

AUTO_END

