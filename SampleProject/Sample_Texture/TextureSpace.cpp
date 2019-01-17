#include "TextureSpace.h"
#include "Config.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"
#include "Input.h"

AUTO_APPLICATION_MAIN(TextureSpace)

void TextureSpace::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void TextureSpace::Start()
{
}
void TextureSpace::Update()
{
	auto input = GetSubSystem<Input>();
	if (input->GetKeyDown(KEY_ESCAPE))
		_engine->ShutDownEngine();
}

void TextureSpace::Stop()
{
}
