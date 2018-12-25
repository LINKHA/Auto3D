#include "ShadowSpace.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"
#include "Level_1.h"


ShadowSpace::ShadowSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ShadowSpace::~ShadowSpace()
{
}

void ShadowSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);

	//ShadowNormal
	RegisterScene(new Level_0(_ambient, 0));
	//ShadowPoint
	//RegisterScene(new Level_1(_ambient, 1));
}

AUTO_APPLICATION_MAIN(ShadowSpace)