#include "Engine.h"
#include "Time.h"
#include "GameObject.h"


#include "FrameBuffersScreen.h"
#include "MSAA.h"
#include "Shadow.h"
#include "../../EngineSetting/Optimize.h"

#include "Input.h"
#include "Renderer.h"
#include "Graphics.h"
#include "BaseSpace.h"
#include "GameWindow.h"


//Temp include
#include "SpriteSort.h"
AUTO_BEGIN

Engine::Engine(Ambient* ambient)
	:Super(ambient)
	, _isExiting(false)
{
	_ambient->RegisterSubSystem(new Renderer(_ambient));
	_ambient->RegisterSubSystem(new Graphics(_ambient));
	_ambient->RegisterSubSystem(new BaseSpace(_ambient));
	_ambient->RegisterSubSystem(new Time(_ambient));
	_ambient->RegisterSubSystem(new Input(_ambient));
	_ambient->RegisterSubSystem(new GameWindow(_ambient));

	//Temp Sub
	_ambient->RegisterSubSystem(new SpriteSort(_ambient));
	//_ambient->RegisterSubSystem(new FrameBuffersScreen(_ambient));
	//_ambient->RegisterSubSystem(new MSAA(_ambient));


	GetSubSystem<GameWindow>()->CreateGameWindow();
}


Engine::~Engine()
{
}

void Engine::Init()
{

	GetSubSystem<BaseSpace>()->Awake();
	GetSubSystem<Graphics>()->Init();

	glEnable(GL_DEPTH);
	glEnable(GL_LESS);

	//if (GetSubSystem<FrameBuffersScreen>()->GetEnable())
	//{
	//	GetSubSystem<FrameBuffersScreen>()->Start();
	//}

//#if MSAA_OPPSCREEN_POINT
//	GetSubSystem<MSAA>()->Start(MSAA_OPPSCREEN_POINT);
//#endif
	GetSubSystem<BaseSpace>()->Start();
}
void Engine::RunFrame()
{
	GetSubSystem<Time>()->Update();
	GetSubSystem<Input>()->Update();
	if (GetSubSystem<Input>()->GetKeyDown(SDLK_ESCAPE))
		_isExiting = true;
	//////////////////////////
//#if MSAA_OPPSCREEN_POINT
//	GetSubSystem<MSAA>()->UpdateStart();
//#endif 

	/*if (GetSubSystem<FrameBuffersScreen>()->GetEnable())
		GetSubSystem<FrameBuffersScreen>()->DrawStart();*/

	GetSubSystem<GameWindow>()->DrawWindow();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	///Accept a buffer bit buffer Bitto specify the buffer to be emptied
	GetSubSystem<BaseSpace>()->Update();
	GetSubSystem<Renderer>()->RenderCameras();
	GetSubSystem<BaseSpace>()->Finish();
	GetSubSystem<Input>()->EndFrame();
	GetSubSystem<GameWindow>()->RunLoopOver();

	//if (GetSubSystem<FrameBuffersScreen>()->GetEnable())
	//	GetSubSystem<FrameBuffersScreen>()->DrawEnd();
//
//#if MSAA_OPPSCREEN_POINT
//	GetSubSystem<MSAA).UpdateEnd();
//#endif 
	
}
void Engine::Exit() 
{
	GetSubSystem<GameWindow>()->DestoryWindow();
}
AUTO_END