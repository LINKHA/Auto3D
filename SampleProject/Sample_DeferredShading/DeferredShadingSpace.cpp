#include "DeferredShadingSpace.h"
#include "Config.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"
#include "Input.h"

AUTO_APPLICATION_MAIN(DeferredShadingSpace)

void DeferredShadingSpace::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void DeferredShadingSpace::Start()
{

}
void DeferredShadingSpace::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}

void DeferredShadingSpace::Stop()
{
}