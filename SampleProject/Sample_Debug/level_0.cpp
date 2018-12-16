#include "Level_0.h"
#include "../FreeCamera.h"
#include "Input.h"
#include "AudioListener.h"
#include "ResourceSystem.h"
#include "AudioBuffer.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}


void Level_0::Start()
{
	auto* audioBuffer = GetSubSystem<ResourceSystem>()->GetResource<AudioBuffer>("sound/SoundTest.wav");

	Node* listenerObj = CreateNode();
	listenerObj->CreateComponent<AudioListener>();
	
	Node* autdieObj = CreateNode();
	audio = autdieObj->CreateComponent<AudioSource>();
	audio->SetAudioBuffer(audioBuffer);

	Node* autdieObj2 = CreateNode();
	audio2 = autdieObj2->CreateComponent<AudioSource>();
	audio2->SetAudioBuffer(audioBuffer);
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

