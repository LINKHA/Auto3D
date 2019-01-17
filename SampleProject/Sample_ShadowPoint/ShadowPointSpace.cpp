#include "ShadowPointSpace.h"
#include "Config.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"
#include "Input.h"

AUTO_APPLICATION_MAIN(ShadowPointSpace)

void ShadowPointSpace::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void ShadowPointSpace::Start()
{
}
void ShadowPointSpace::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}

void ShadowPointSpace::Stop()
{
}
