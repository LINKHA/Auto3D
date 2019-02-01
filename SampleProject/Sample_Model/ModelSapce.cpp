#include "ModelSapce.h"
#include "Config.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "level_0.h"
#include "Input.h"

AUTO_APPLICATION_MAIN(ModelSapce)

void ModelSapce::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void ModelSapce::Start()
{

}
void ModelSapce::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}

void ModelSapce::Stop()
{
}