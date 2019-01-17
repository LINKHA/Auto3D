#include "UISapce.h"
#include "Config.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Input.h"
#include "level_0.h"

AUTO_APPLICATION_MAIN(UISapce)

void UISapce::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void UISapce::Start()
{
}
void UISapce::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}


void UISapce::Stop()
{
}