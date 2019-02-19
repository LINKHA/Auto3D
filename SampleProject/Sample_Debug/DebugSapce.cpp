#include "DebugSapce.h"
#include "Config.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Input.h"

AUTO_APPLICATION_MAIN(DebugSapce)

void DebugSapce::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
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