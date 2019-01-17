#include "Physics2DSapce.h"
#include "Config.h"
#include "Application.h"
#include "FileSystem.h"
#include "Level_0.h"
#include "ResourceSystem.h"

AUTO_APPLICATION_MAIN(Physics2DSapce)
void Physics2DSapce::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void Physics2DSapce::Start()
{
}
void UISapce::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}

void Physics2DSapce::Stop()
{
}
