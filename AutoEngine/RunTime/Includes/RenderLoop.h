#ifndef RENDER_LOOP_H_
#define RENDER_LOOP_H_
#include "Camera.h"
AUTO_BEGIN
class RenderLoop;

struct RenderLoopContext  
{
	Camera*			m_Camera;
	RenderLoop*		m_RenderLoop; 

	int				m_RenderQueueStart;
	int				m_RenderQueueEnd;
};
class RenderLoop
{
private:
	RenderLoopContext m_Context;
public:
	RenderLoop(Camera& camera);
	~RenderLoop();
};

RenderLoop* CreateRenderLoop(Camera& camera);
void DeleteRenderLoop(RenderLoop* loop);
void RunRenderLoop(RenderLoop& loop);
void CleanUpAfterRenderLoop(RenderLoop& loop);

AUTO_END
#endif // !RENDER_LOOP_H_