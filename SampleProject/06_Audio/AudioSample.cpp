#include "AudioSample.h"

void AudioSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Audio Sample");

}
void AudioSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();

	scene = AObject::Create<AScene>();
	scene->CreateChild<AOctree>();
	auto sound = cache->LoadResource<ASound>("Sound/SoundTest.wav");
	listener = scene->CreateChild<AAudioListener>();

	source1 = scene->CreateChild<AAudioSource>();
	source1->SetSound(sound);
	source2 = scene->CreateChild<AAudioSource>();
	source2->SetSound(sound);
}
void AudioSample::Update()
{
	Super::Update();
}


void AudioSample::Stop()
{
	Super::Stop();
}
void AudioSample::UIDraw()
{
	GUI::Begin("Sound 1");
	if (GUI::Button("Play"))
		source1->Play();
	if (GUI::Button("Pause"))
		source1->Pause();
	if (GUI::Button("Stop"))
		source1->Stop();
	if (GUI::Button("Rewind"))
		source1->Rewind();
	GUI::End();

	GUI::Begin("Sound 2");
	if (GUI::Button("Play"))
		source2->Play();
	if (GUI::Button("Pause"))
		source2->Pause();
	if (GUI::Button("Stop"))
		source2->Stop();
	if (GUI::Button("Rewind"))
		source2->Rewind();
	GUI::End();
}

AUTO_APPLICATION_MAIN(AudioSample)