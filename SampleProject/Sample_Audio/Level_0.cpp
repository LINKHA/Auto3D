#include "Level_0.h"
#include "GameObject.h"
#include "../FreeCamera.h"
#include "Input.h"
#include "AudioListener.h"
Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}


void Level_0::Start()
{
	GameObject* listenerObj = new GameObject(_ambient, _levelNumber);
	AudioListener* listener = new AudioListener(_ambient);
	listenerObj->AddComponent(listener);

	GameObject* autdieObj = new GameObject(_ambient, _levelNumber);
	audio = new AudioSource(_ambient,new AudioBuffer(_ambient,"../Resource/sound/SoundTest.wav"));
	autdieObj->AddComponent(audio);
	
	GameObject* autdieObj2 = new GameObject(_ambient, _levelNumber);
	audio2 = new AudioSource(_ambient, new AudioBuffer(_ambient, "../Resource/sound/SoundTest.wav"));
	autdieObj->AddComponent(audio2);
}

void Level_0::Update()
{
	if(GetSubSystem<Input>()->GetKeyPress(KEY_A) && audio->GetState()!= AudioSourceState::PLAYING)
		audio->Play(1000);
	if (GetSubSystem<Input>()->GetKeyPress(KEY_S))
		audio->Pause();
	if(GetSubSystem<Input>()->GetKeyPress(KEY_D))
		audio->Stop();
	if (GetSubSystem<Input>()->GetKeyPress(KEY_F))
		audio->Rewind();

	if (GetSubSystem<Input>()->GetKeyPress(KEY_Q) && audio2->GetState() != AudioSourceState::PLAYING)
		audio2->Play(1000);
}

