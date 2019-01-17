#include "DepthTestSpace.h"
#include "Config.h"
#include "Level_0.h"
#include "FileSystem.h"
#include "ResourceSystem.h"
#include "Input.h"

AUTO_APPLICATION_MAIN(DepthTestSpace)

void DepthTestSpace::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void DepthTestSpace::Start()
{

}
void DepthTestSpace::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}

void DepthTestSpace::Stop()
{
}