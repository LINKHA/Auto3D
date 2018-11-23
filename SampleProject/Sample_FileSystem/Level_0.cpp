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
	OutPutString(GetSubSystem<FileSystem>()->GetUserDocumentsDir().c_str());
	//Out put console
	AutoWCout << GetSubSystem<FileSystem>()->GetUserDocumentsDir() << AutoEndl;
	//Out put program dir in ouput console
	AutoWCout << GetSubSystem<FileSystem>()->GetProgramDir() << AutoEndl;
	//Out put current dir in ouput console
	AutoWCout << GetSubSystem<FileSystem>()->GetCurrentDir() << AutoEndl;
	//Out put temporary dir in ouput console
	AutoWCout << GetSubSystem<FileSystem>()->GetTemporaryDir() << AutoEndl;
	
}

void Level_0::Update()
{
	SceneSuper::Update();
}

