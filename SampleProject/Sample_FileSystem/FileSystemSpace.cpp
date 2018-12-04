#include "FileSystemSpace.h"
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
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(FileSystemSpace)