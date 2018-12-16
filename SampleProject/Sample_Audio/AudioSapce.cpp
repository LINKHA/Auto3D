#include "AudioSapce.h"
#include "../FreeCamera.h"
#include "ResourceSystem.h"
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
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}

void AudioSapce::Start()
{
}
void AudioSapce::Update()
{
}
AUTO_APPLICATION_MAIN(AudioSapce)