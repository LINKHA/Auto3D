void Sample::Init()
{
}
void Sample::Start()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	scene2d = new Scene2D();
	uiCamera = scene2d->CreateChild<Camera2D>();
	uiCamera->SetOrthographic(true);
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, -100.0f));
	Subsystem<RegisteredBox>()->RegisterScene2D(scene2d, uiCamera);

	CreateLogo();
}
void Sample::Update()
{
	if (Object::Subsystem<Input>()->IsKeyPress(KEY_ESCAPE))
		// Closing the render window responds to the engine closing
		Object::Subsystem<Graphics>()->RenderWindow()->Close();
}

void Sample::Stop()
{
}
void Sample::CreateLogo()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	Sprite2D* logoLong = scene2d->CreateChild<Sprite2D>();
	logoLong->SetTexture(cache->LoadResource<Texture>("logoLong.png"));
	logoLong->SetScale(Vector3F(3.0f, 0.8f, 1.0f));
	logoLong->SetPosition(Vector3F(7.0f, -9.2f, -10.0f));
}