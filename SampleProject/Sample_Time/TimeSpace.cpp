#include "TimeSpace.h"
#include "Config.h"
#include "Level_0.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Input.h"

AUTO_APPLICATION_MAIN(TimeSpace)

void TimeSpace::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void TimeSpace::Start()
{
}
void TimeSpace::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}

void TimeSpace::Stop()
{
}
