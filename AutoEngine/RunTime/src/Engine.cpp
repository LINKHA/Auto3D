#include "Engine.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "Shader.h"
#include "BaseSpace.h"
#include "RenderManager.h"
#include "FrameBuffersScreen.h"
#include "MSAA.h"
#include "Shadow.h"
#include "../../EngineSetting/Optimize.h"
#include "GLDebug.h"
#include "Input.h"
AUTO_BEGIN

Engine::Engine(Ambient* ambient)
	:Super(ambient)
	, _isExiting(false)
{
}


Engine::~Engine()
{
}

void Engine::Init()
{

	
	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_FRAMEBUFFER_SRGB);

	INSTANCE(BaseSpace).Start();

	if (INSTANCE(FrameBuffersScreen).GetEnable())
	{
		INSTANCE(FrameBuffersScreen).Start();
	}

#if MSAA_OPPSCREEN_POINT
	INSTANCE(MSAA).Start(MSAA_OPPSCREEN_POINT);
#endif
	INSTANCE(BaseSpace).Awake();


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
	INSTANCE(BaseSpace).Update();
	INSTANCE(RenderManager).RenderCameras();

	INSTANCE(BaseSpace).Finish();
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