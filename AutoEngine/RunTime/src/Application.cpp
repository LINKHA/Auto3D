#include "Application.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "TextMesh.h"
#include "Shader.h"
#include "MotionSpace.h"
AUTO_BEGIN

SINGLETON_INSTANCE(Application);

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
	

	stbi_set_flip_vertically_on_load(true);

	//Print(Monitors::Instance().getMonitorsCount());
	//Print(Monitors::Instance().getMonitorsWidthIndex(1));

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ErrorString("Failed to initialize GLAD from Engine\n");
		return AU_ERROR;
	}
	glEnable(GL_DEPTH_TEST);
	MotionSpace::Instance().SetWindow(glfwWindow);
	MotionSpace::Instance().Awake();

	return AU_NORMAL;
}



int Application::RunLoop()
{
	MotionSpace::Instance().Start();

	while (!GrShouldCloseWindow(glfwWindow))
	{
		TimeManager::Instance().Update();

		//LogString(TimeManager::Instance().GetRealTime().Second);
		//Print(TimeManager::Instance().GetCurTime());
		//Print(TimeManager::Instance().GetDeltaTime());
		//////////////////////////
		
		window.DrawWindow();
		///Accept a buffer bit buffer Bitto specify the buffer to be emptied
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MotionSpace::Instance().Update();

	
		//////////////////////////////////////////////////////////////////////////
		window.RunLoopOver();
		//////////////////////////////////////////////////////////////////////////
		MotionSpace::Instance().Finish();
	}


	return AU_NORMAL;
}
int Application::Finish()
{

	window.DestoryWindow();
	glfwWindow = nullptr;
	return AU_NORMAL;
}

Application::Application()
{
	glfwWindow = window.CreateGameWindow();

}
AUTO_END
