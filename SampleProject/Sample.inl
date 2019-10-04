void Sample::Init()
{
	auto* graphics = Object::Module<Graphics>();
	// Set window mode
	WindowModeDesc& windowModeDesc = graphics->RenderWindow()->ModeDesc();
	windowModeDesc._size = RectI(0, 0, 1024, 768);
	windowModeDesc._multisample = 4;
	windowModeDesc._fullscreen = false;
	windowModeDesc._resizable = true;
	windowModeDesc._center = true;
	windowModeDesc._borderless = false;
	windowModeDesc._highDPI = false;
}
void Sample::Start()
{
	auto* cache = Object::Module<ResourceCache>();
	scene2d = new Scene2D();
	uiCamera = scene2d->CreateChild<Camera2D>();
	uiCamera->SetOrthographic(true);
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, -100.0f));

	CreateLogo();
}
void Sample::Update()
{
	if (Object::Module<Input>()->IsKeyPress(KEY_ESCAPE))
		// Closing the render window responds to the engine closing
		Object::Module<Graphics>()->RenderWindow()->Close();
}

void Sample::Stop()
{
}
void Sample::CreateLogo()
{
	auto* cache = Object::Module<ResourceCache>();
	Sprite2D* logoLong = scene2d->CreateChild<Sprite2D>();
	logoLong->SetTexture(cache->LoadResource<Texture>("Texture/logoLong.png"));
	logoLong->SetScale(Vector3F(3.0f, 0.8f, 1.0f));
	logoLong->SetPosition(Vector3F(7.0f, -9.2f, -10.0f));
}