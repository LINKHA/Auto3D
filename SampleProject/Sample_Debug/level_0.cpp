#include "Level_0.h"
#include "Light.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "FileSystem.h"

void Level_0::Awake()
{
	SceneSuper::Awake();
}
void Level_0::Start()
{
	SceneSuper::Start();
	//Out put user documents dir in output console
	Print(GetSubSystem<FileSystem>()->GetUserDocumentsDir().CStr());
	//Out put console
	Print(GetSubSystem<FileSystem>()->GetUserDocumentsDir().CStr());
	//Out put temporary dir in output console
	Print(GetSubSystem<FileSystem>()->GetTemporaryDir().CStr());
	//Out put program dir in output console
	Print(GetSubSystem<FileSystem>()->GetProgramDir().CStr());
	//Out put current dir in output console
	Print(GetSubSystem<FileSystem>()->GetCurrentDir().CStr());
}

void Level_0::Update()
{
	SceneSuper::Update();
}

