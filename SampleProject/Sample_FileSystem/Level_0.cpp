#include "Level_0.h"
#include "Light.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "FileSystem.h"

Level_0::Level_0(Ambient* ambient, int id)
	:LevelScene(ambient, id)
{}
void Level_0::Awake()
{
	SceneSuper::Awake();
}
void Level_0::Start()
{
	SceneSuper::Start();
	//Out put user documents dir in ouput console
	Print(GetSubSystem<FileSystem>()->GetUserDocumentsDir().CStr());
	//Out put console
	Print(GetSubSystem<FileSystem>()->GetUserDocumentsDir().CStr());
	//Out put temporary dir in ouput console
	Print(GetSubSystem<FileSystem>()->GetTemporaryDir().CStr());
	//Out put program dir in ouput console
	Print(GetSubSystem<FileSystem>()->GetProgramDir().CStr());
	//Out put current dir in ouput console
	Print(GetSubSystem<FileSystem>()->GetCurrentDir().CStr());
	
}

void Level_0::Update()
{
	SceneSuper::Update();
}

