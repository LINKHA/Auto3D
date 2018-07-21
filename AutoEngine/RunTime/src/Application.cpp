#include "Application.h"
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
	try 
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
	catch (std::bad_alloc&)
	{
		ErrorString("An application that has an out-of-memory condition will exit immediately.");
		return EXIT_FAILURE;
	}
}

int Application::Init()
{
	INSTANCE(BaseSpace).Awake();
	glfwWindowHint(GLFW_SAMPLES, 4);
	//stbi_set_flip_vertically_on_load(true);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ErrorString("Failed to initialize GLAD from Engine\n");
		return AU_ERROR;
	}


	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_FRAMEBUFFER_SRGB);
#if MSAA_POINT
	glEnable(GL_MULTISAMPLE);
#endif // MSAA_POINT
	


	return AU_NORMAL;
}



int Application::RunLoop()
{
#if MSAA_OPPSCREEN_POINT
	INSTANCE(MSAA).Start(MSAA_OPPSCREEN_POINT);
#endif
	INSTANCE(BaseSpace).Start();

	if (INSTANCE(FrameBuffersScreen).GetEnable())
	{
		INSTANCE(FrameBuffersScreen).Start();
	}
	while (!GrShouldCloseWindow(glfwWindow))
	{
		
		INSTANCE(TimeManager).Update();
		//////////////////////////
#if MSAA_OPPSCREEN_POINT
		INSTANCE(MSAA).UpdateStart();
#endif 

		if (INSTANCE(FrameBuffersScreen).GetEnable())
			INSTANCE(FrameBuffersScreen).DrawStart();
		
		
		INSTANCE(GLWindow).DrawWindow();
		///Accept a buffer bit buffer Bitto specify the buffer to be emptied
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		INSTANCE(RenderManager).RenderCameras();
		INSTANCE(BaseSpace).Update();

		INSTANCE(GLWindow).RunLoopOver();
		INSTANCE(BaseSpace).Finish();

		if (INSTANCE(FrameBuffersScreen).GetEnable())
			INSTANCE(FrameBuffersScreen).DrawEnd();
		
#if MSAA_OPPSCREEN_POINT
		INSTANCE(MSAA).UpdateEnd();
#endif 
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
