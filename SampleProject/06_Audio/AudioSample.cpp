#include "AudioSample.h"

void AudioSample::Init()
{
	Super::Init();
	auto* graphics = Object::Module<Graphics>();
	graphics->RenderWindow()->SetTitle("Audio Sample");

}
void AudioSample::Start()
{
	Super::Start();
	auto* cache = Object::Module<ResourceCache>();

	canvas = new Canvas();
	canvas->SetTitle("Sound 1");

	button = canvas->CreateChild<Button>();
	button->SetText("Play");

	button2 = canvas->CreateChild<Button>();
	button2->SetText("Pause");

	button3 = canvas->CreateChild<Button>();
	button3->SetText("Stop");

	button4 = canvas->CreateChild<Button>();
	button4->SetText("Rewind");

	canvas2 = new Canvas();
	canvas2->SetTitle("Sound 2");

	button5 = canvas2->CreateChild<Button>();
	button5->SetText("Play");

	button6 = canvas2->CreateChild<Button>();
	button6->SetText("Pause");

	button7 = canvas2->CreateChild<Button>();
	button7->SetText("Stop");

	button8 = canvas2->CreateChild<Button>();
	button8->SetText("Rewind");

	scene = new Scene();
	scene->CreateChild<Octree>();
	auto sound = cache->LoadResource<Sound>("SoundTest.wav");
	listener = scene->CreateChild<AudioListener>();

	source1 = scene->CreateChild<AudioSource>();
	source1->SetSound(sound);
	source2 = scene->CreateChild<AudioSource>();
	source2->SetSound(sound);
}
void AudioSample::Update()
{
	Super::Update();
	if(button->IsClick())
		source1->Play();
	if(button2->IsClick())
		source1->Pause();
	if (button3->IsClick())
		source1->Stop();
	if (button4->IsClick())
		source1->Rewind();

	if (button5->IsClick())
		source2->Play();
	if (button6->IsClick())
		source2->Pause();
	if (button7->IsClick())
		source2->Stop();
	if (button8->IsClick())
		source2->Rewind();
}

void AudioSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(AudioSample)