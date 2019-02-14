#include "DebugSapce.h"
#include "Config.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Input.h"
#include "level_0.h"

AUTO_APPLICATION_MAIN(DebugSapce)

void DebugSapce::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<tScene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void DebugSapce::Start()
{
}
void DebugSapce::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}

void DebugSapce::Stop()
{
}