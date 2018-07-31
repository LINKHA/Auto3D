#pragma once
#include "Camera.h"
#include "Object.h"
namespace Auto3D {

class RenderLoop : public Object
{
	REGISTER_DERIVED_CLASS(RenderLoop, Object);
	DECLARE_OBJECT_SERIALIZE(RenderLoop);
private:
	Camera *		_camera;
	int				_renderQueueStart;
	int				_renderQueueEnd;
public:
	RenderLoop(Ambient* ambient,Camera& camera);
	void RunLoop();
};

RenderLoop* CreateRenderLoop(Ambient* ambient,Camera& camera);
void DeleteRenderLoop(RenderLoop* loop);
void CleanUpAfterRenderLoop(RenderLoop& loop);

}
