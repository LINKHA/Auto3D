#include "AudioSample.h"

void AudioSample::Init()
{
	Super::Init();
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("Audio Sample");

}
void AudioSample::Start()
{
	Super::Start();
	auto* cache = Object::Subsystem<ResourceCache>();

	Sprite* bakcground = canvas->CreateChild<Sprite>();
	bakcground->SetTexture(cache->LoadResource<Texture>("HelloWorld.png"));
	bakcground->SetScale(Vector3F(20.0f, 15.0f, 1.0f));

	Sprite* logo = canvas->CreateChild<Sprite>();
	logo->SetTexture(cache->LoadResource<Texture>("Newlogo.png"));
	logo->SetPosition(Vector3F(0.0f, 0.0f, -0.1f));
	logo->SetScale(Vector3F(3.0f, 3.0f, 1.0f));

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	// Register scene to scene system use to render
	Object::Subsystem<RegisteredBox>()->RegisterScene(scene, camera);
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
	auto* input = Object::Subsystem<Input>();
	if (input->IsKeyDown(KEY_A) && source1->GetState() != AudioSourceState::Playing)
		source1->Play(0);
	if (input->IsKeyDown(KEY_S))
		source1->Pause();
	if (input->IsKeyDown(KEY_D))
		source1->Stop();
	if (input->IsKeyDown(KEY_F))
		source1->Rewind();

	if (input->IsKeyDown(KEY_Q) && source2->GetState() != AudioSourceState::Playing)
		source2->Play(1000);
}

void AudioSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(AudioSample)