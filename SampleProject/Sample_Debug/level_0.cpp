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

	GetSubSystem<FileSystem>()->SetCurrentDir("C:\\Users\\LKH\\Desktop");
	GetSubSystem<FileSystem>()->CreateDir("dir");
}

void Level_0::Update()
{
	SceneSuper::Update();
}

