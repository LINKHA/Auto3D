#pragma once
#include "Camera.h"
AUTO_BEGIN
class RenderLoop;

struct RenderLoopContext  
{
	Camera*			camera;
	RenderLoop*		renderLoop; 

	int				renderQueueStart;
	int				renderQueueEnd;
};
class RenderLoop
{
private:
	RenderLoopContext _context;
public:
	RenderLoop(Camera& camera);
	void RunLoop();
	~RenderLoop();
};

RenderLoop* CreateRenderLoop(Camera& camera);
void DeleteRenderLoop(RenderLoop* loop);
void CleanUpAfterRenderLoop(RenderLoop& loop);

AUTO_END
