#include "AudioSapce.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Level_0.h"
AudioSapce::AudioSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
AudioSapce::~AudioSapce()
{
	RemoveLevel(0);
}

void AudioSapce::Start()
{
	RegisterLevel(new Level_0(_ambient, 0));
}
void AudioSapce::Update()
{
}
int AudioSapce::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(AudioSapce)