#include "VertexExplodeSpace.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"

VertexExplodeSpace::VertexExplodeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}

VertexExplodeSpace::~VertexExplodeSpace()
{
	RemoveScene(0);
}

void VertexExplodeSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	STRING ResourceDir2 = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Sample_VertexExplode/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir2);

	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(VertexExplodeSpace)