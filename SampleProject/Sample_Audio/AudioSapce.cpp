#include "AudioSapce.h"
#include "../FreeCamera.h"
#include "Level_0.h"
AudioSapce::AudioSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
AudioSapce::~AudioSapce()
{
	RemoveScene(0);
}

void AudioSapce::Start()
{
	RegisterScene(new Level_0(_ambient, 0));
}
void AudioSapce::Update()
{
}
AUTO_APPLICATION_MAIN(AudioSapce)