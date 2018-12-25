#include "ModelSapce.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "level_0.h"
ModelSapce::ModelSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
ModelSapce::~ModelSapce()
{
	RemoveScene(0);
}
void ModelSapce::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}


AUTO_APPLICATION_MAIN(ModelSapce)