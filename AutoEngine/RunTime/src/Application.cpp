#include "Application.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "Shader.h"
#include "BaseSpace.h"
#include "RenderManager.h"
#include "FrameBuffersScreen.h"
AUTO_BEGIN

SINGLETON_INSTANCE(Application);

GLFWwindow* glfwWindow;

Application::~Application()
{
}
/*
* @Application::run
* this is Engine important funcation,
* Determine the number of frames based on the speed of this function
* Run once per frame
*/
int Application::Run()
{
	if (Init() == AU_ERROR)
	{
		return AU_ERROR;
	}
	if (RunLoop() == AU_ERROR)
	{
		return AU_ERROR;
	}
	return Finish();
}

int Application::Init()
{
	INSTANCE(BaseSpace).Awake();

	stbi_set_flip_vertically_on_load(true);

	//Print(Monitors::Instance().getMonitorsCount());
	//Print(Monitors::Instance().getMonitorsWidthIndex(1));

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ErrorString("Failed to initialize GLAD from Engine\n");
		return AU_ERROR;
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	return AU_NORMAL;
}



int Application::RunLoop()
{
	INSTANCE(BaseSpace).Start();
	if (INSTANCE(FrameBuffersScreen).GetEnable())
	{
		INSTANCE(FrameBuffersScreen).Start();
	}
	while (!GrShouldCloseWindow(glfwWindow))
	{
		
		INSTANCE(TimeManager).Update();

		//LogString(TimeManager::Instance().GetRealTime().Second);
		//Print(TimeManager::Instance().GetCurTime());
		//Print(TimeManager::Instance().GetDeltaTime());
		//////////////////////////
		if (INSTANCE(FrameBuffersScreen).GetEnable())
		{
			INSTANCE(FrameBuffersScreen).DrawStart();
		}
		INSTANCE(GLWindow).DrawWindow();
		///Accept a buffer bit buffer Bitto specify the buffer to be emptied
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		INSTANCE(RenderManager).RenderCameras();
		//////////////////////////////////////////////////////////////////////////
		INSTANCE(GLWindow).RunLoopOver();
		//////////////////////////////////////////////////////////////////////////
		INSTANCE(BaseSpace).Finish();
		if (INSTANCE(FrameBuffersScreen).GetEnable())
		{
			INSTANCE(FrameBuffersScreen).DrawEnd();
		}
	}


	return AU_NORMAL;
}
int Application::Finish()
{

	INSTANCE(GLWindow).DestoryWindow();
	glfwWindow = nullptr;
	return AU_NORMAL;
}

Application::Application()
{
	INSTANCE(GLWindow).CreateGameWindow();
	glfwWindow = INSTANCE(GLWindow).GetGLWindow();
}
AUTO_END
