#include "FileSystemSpace.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"

FileSystemSpace::FileSystemSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
FileSystemSpace::~FileSystemSpace()
{
	RemoveScene(0);
}

void FileSystemSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(FileSystemSpace)