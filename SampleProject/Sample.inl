void Sample::Init()
{
}
void Sample::Start()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	canvas = new Canvas();
	uiCamera = canvas->CreateChild<UICamera>();
	uiCamera->SetOrthographic(true);
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, -100.0f));
	Subsystem<RegisteredBox>()->RegisterCanvas(canvas, uiCamera);

	//CreateLogo();
}
void Sample::Update()
{
	if (Object::Subsystem<Input>()->IsKeyPress(27))
		Object::Subsystem<Graphics>()->RenderWindow()->Close();
}

void Sample::Stop()
{
}
void Sample::CreateLogo()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	Sprite* logoLong = canvas->CreateChild<Sprite>();
	logoLong->SetTexture(cache->LoadResource<Texture>("logoLong.png"));
	logoLong->SetScale(Vector3F(3.0f, 0.8f, 1.0f));
	logoLong->SetPosition(Vector3F(7.0f, -9.2f, -10.0f));
}