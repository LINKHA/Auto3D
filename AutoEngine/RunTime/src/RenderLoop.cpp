#include "RenderLoop.h"
#include "Renderer.h"
#include "BaseSpace.h"
#include "Graphics.h"
#include "BaseSpace.h"
namespace Auto3D {

RenderLoop::RenderLoop(Ambient* ambient,Camera& camera)
	:Super(ambient)
{
	_camera = &camera;
	GetSubSystem<Renderer>()->AddCamera(&camera);
}

RenderLoop::~RenderLoop()
{
}

void RenderLoop::RunLoop()
{
	//Camera * cam = _camera;
	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
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

}

