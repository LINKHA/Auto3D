void Sample::Init()
{
	auto* graphics = GModuleManager::Get().GraphicsModule();
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
	auto* cache = GModuleManager::Get().CacheModule();
	scene2d = new Scene2D();

	//Do not set this Layer or tag, which is generated automatically inside the engine. 
	//Customizations need to start at index 1
	/*scene2d->DefineLayer(0, "Default");*/
	/*scene2d->DefineTag(0, "None");*/

	scene2d->DefineLayer(1, "UI");

	scene2d->DefineTag(1, "Player");
	scene2d->DefineTag(2, "Enemy");

	logoCamera = scene2d->CreateChild<Camera2D>();
	logoCamera->SetOrthographic(true);
	logoCamera->SetPosition(Vector3F(0.0f, 0.0f, -100.0f));
	logoCamera->SetLayoutMaskAll();
	logoCamera->SetLayoutMaskOutName("UI");

	CreateLogo();
}
void Sample::Update()
{
	if (GModuleManager::Get().InputModule()->IsKeyPress(KEY_ESCAPE))
	{
		// Closing the render window responds to the engine closing
		GModuleManager::Get().GraphicsModule()->RenderWindow()->Close();
	}
}

void Sample::Stop()
{
}
void Sample::CreateLogo()
{
	auto* cache = GModuleManager::Get().CacheModule();
	Sprite2D* logoLong = scene2d->CreateChild<Sprite2D>();
	logoLong->SetTexture(cache->LoadResource<Texture>("Texture/logoLong.png"));
	logoLong->SetScale(Vector3F(3.0f, 0.8f, 1.0f));
	logoLong->SetPosition(Vector3F(7.0f, -9.2f, -10.0f));
	logoLong->SetLayerName("UI");

}