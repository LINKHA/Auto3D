#include "Level_0.h"
#include "../FreeCamera.h"
#include "Input.h"
#include "AudioListener.h"

void Level_0::Start()
{
	Node* listenerObj = CreateNode();
	AudioListener* listener = new AudioListener(_ambient);
	listenerObj->AddComponent(listener);

	Node* autdieObj = CreateNode();
	audio = new AudioSource(_ambient,new _AudioBuffer(_ambient,"../Resource/sound/SoundTest.wav"));
	autdieObj->AddComponent(audio);
	
	Node* autdieObj2 = CreateNode();
	audio2 = new AudioSource(_ambient, new _AudioBuffer(_ambient, "../Resource/sound/SoundTest.wav"));
	autdieObj->AddComponent(audio2);
}

void Level_0::Update()
{
	if (GetSubSystem<Input>()->GetKeyDown(KEY_A) && audio->GetState() != AudioSourceState::PLAYING)
		audio->Play(0);
	if (GetSubSystem<Input>()->GetKeyDown(KEY_S))
		audio->Pause();
	if (GetSubSystem<Input>()->GetKeyDown(KEY_D))
		audio->Stop();
	if (GetSubSystem<Input>()->GetKeyDown(KEY_F))
		audio->Rewind();

	if (GetSubSystem<Input>()->GetKeyDown(KEY_Q) && audio2->GetState() != AudioSourceState::PLAYING)
		audio2->Play(1000);
}

