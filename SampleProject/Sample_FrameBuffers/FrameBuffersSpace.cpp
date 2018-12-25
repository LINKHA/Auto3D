#include "FrameBuffersSpace.h"
#include "Level_0.h"
#include "FileSystem.h"
#include "ResourceSystem.h"

FrameBuffersSpace::FrameBuffersSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
FrameBuffersSpace::~FrameBuffersSpace()
{
	RemoveScene(0);
}

void FrameBuffersSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}
AUTO_APPLICATION_MAIN(FrameBuffersSpace)
