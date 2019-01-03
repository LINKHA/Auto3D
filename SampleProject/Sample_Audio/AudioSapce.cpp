#include "AudioSapce.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "level_0.h"


AUTO_APPLICATION_MAIN(AudioSapce)

void AudioSapce::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void AudioSapce::Start()
{

}
void AudioSapce::Stop()
{
}