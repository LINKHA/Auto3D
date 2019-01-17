#include "AntiAliasingSpace.h"
#include "Config.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"
#include "Input.h"

AUTO_APPLICATION_MAIN(AntiAliasingSpace)

void AntiAliasingSpace::Init()
{
	STRING ResourceDir = system_content_dictionary;
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void AntiAliasingSpace::Start()
{
}
void AntiAliasingSpace::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}

void AntiAliasingSpace::Stop()
{
}
