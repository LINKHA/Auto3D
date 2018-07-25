#include "Engine.h"
#include "TimeManager.h"
#include "GameObject.h"

#include "RenderManager.h"
#include "FrameBuffersScreen.h"
#include "MSAA.h"
#include "Shadow.h"
#include "../../EngineSetting/Optimize.h"

#include "Input.h"

#include "Renderer.h"
#include "Graphics.h"
#include "BaseSpace.h"
AUTO_BEGIN

Engine::Engine(Ambient* ambient)
	:Super(ambient)
	, _isExiting(false)
{
	_ambient->RegisterSubSystem(new Renderer(_ambient));
	_ambient->RegisterSubSystem(new Graphics(_ambient));
	_ambient->RegisterSubSystem(new BaseSpace(_ambient));
}


Engine::~Engine()
{
}

void Engine::Init()
{

	GetSubSystem<BaseSpace>()->Awake();
	GetSubSystem<Graphics>()->Init();

	

	if (INSTANCE(FrameBuffersScreen).GetEnable())
	{
		INSTANCE(FrameBuffersScreen).Start();
	}

#if MSAA_OPPSCREEN_POINT
	INSTANCE(MSAA).Start(MSAA_OPPSCREEN_POINT);
#endif
	GetSubSystem<BaseSpace>()->Start();
}
void Engine::RunFrame()
{
	INSTANCE(TimeManager).Update();
	INSTANCE(Input).Update();
	if (INSTANCE(Input).GetKeyDown(SDLK_ESCAPE))
		_isExiting = true;
	//////////////////////////
#if MSAA_OPPSCREEN_POINT
	INSTANCE(MSAA).UpdateStart();
#endif 

	if (INSTANCE(FrameBuffersScreen).GetEnable())
		INSTANCE(FrameBuffersScreen).DrawStart();

	INSTANCE(GLWindow).DrawWindow();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	///Accept a buffer bit buffer Bitto specify the buffer to be emptied
	GetSubSystem<BaseSpace>()->Update();
	INSTANCE(RenderManager).RenderCameras();
	GetSubSystem<BaseSpace>()->Finish();
	
	INSTANCE(Input).EndFrame();
	INSTANCE(GLWindow).RunLoopOver();

	if (INSTANCE(FrameBuffersScreen).GetEnable())
		INSTANCE(FrameBuffersScreen).DrawEnd();

#if MSAA_OPPSCREEN_POINT
	INSTANCE(MSAA).UpdateEnd();
#endif 
	
}
void Engine::Exit() 
{
	INSTANCE(GLWindow).DestoryWindow();
}
AUTO_END