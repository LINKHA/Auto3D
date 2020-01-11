#include "HelloWorldSample.h"
#include <bgfx/bgfx.h>

void HelloWorldSample::Init()
{
	Super::Init();
}
void HelloWorldSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();

	ACamera2D* camera = scene2d->CreateChildNode<ACamera2D>();
	camera->SetOrthographic(true);
	camera->SetPosition(TVector3F(0.0f, 0.0f, -100.0f));
	camera->SetLayoutMaskName("UI");

	ASprite2D* bakcground = scene2d->CreateChildNode<ASprite2D>();
	bakcground->SetTexture(cache->LoadResource<ATexture>("Texture/HelloWorld.png"));
	bakcground->SetPosition(TVector3F(0.0f, 0.0f, 10.1f));
	bakcground->SetScale(TVector3F(20.0f, 15.0f, 1.0f));

	ASprite2D* logo = scene2d->CreateChildNode<ASprite2D>();
	logo->SetTexture(cache->LoadResource<ATexture>("Texture/Newlogo.png"));
	logo->SetPosition(TVector3F(0.0f, 0.0f, -0.1f));
	logo->SetScale(TVector3F(3.0f, 3.0f, 1.0f));
}
void HelloWorldSample::Update()
{
	Super::Update();
}

void HelloWorldSample::Stop()
{
	Super::Stop();
}
AUTO_APPLICATION_MAIN(HelloWorldSample)