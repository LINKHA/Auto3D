#include "AudioSapce.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "level_0.h"

AudioSapce::AudioSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
AudioSapce::~AudioSapce()
{
	RemoveScene(0);
}
void AudioSapce::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

void AudioSapce::Start()
{
}
void AudioSapce::Update()
{
}
AUTO_APPLICATION_MAIN(AudioSapce)