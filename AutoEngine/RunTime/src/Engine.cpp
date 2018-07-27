#include "Engine.h"

#include "GameObject.h"
#include "Shadow.h"
#include "../../EngineSetting/Optimize.h"

#include "Input.h"
#include "Renderer.h"
#include "Graphics.h"
#include "BaseSpace.h"
#include "GameWindow.h"
#include "Time.h"

//Temp include
#include "SpriteSort.h"
#include "FrameBuffersScreen.h"
#include "MSAA.h"
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
	auto* baseSpace = GetSubSystem<BaseSpace>();
	auto* input = GetSubSystem<Input>();
	auto* time = GetSubSystem<Time>();
	auto* window = GetSubSystem<GameWindow>();
	auto* renderer = GetSubSystem<Renderer>();

	time->Update();
	input->Update();
	if (input->GetKeyDown(KEY_ESCAPE))
		_isExiting = true;
	//////////////////////////
//#if MSAA_OPPSCREEN_POINT
//	GetSubSystem<MSAA>()->UpdateStart();
//#endif 

	/*if (GetSubSystem<FrameBuffersScreen>()->GetEnable())
		GetSubSystem<FrameBuffersScreen>()->DrawStart();*/

	
	
	
	///Accept a buffer bit buffer Bitto specify the buffer to be emptied

	baseSpace->Update();

	Render();
	
	///End
	baseSpace->Finish();
	input->EndFrame();
	window->RunLoopOver();

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
void Engine::Render()
{
	auto* graphics = GetSubSystem<Graphics>();
	if (!graphics->BeginFrame())
		return;

	
	GetSubSystem<Renderer>()->Render();

	graphics->EndFrame();
}
AUTO_END