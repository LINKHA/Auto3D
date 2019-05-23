#include "GUISample.h"

void GUISample::Init()
{
	Super::Init();
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("GUI Sample");
}

void GUISample::Start()
{
	Super::Start();

	canvas = new Canvas();
	canvas->SetTitle("Hello GUI"); 
	canvas->SetCloseButtonEnable(true);

	Text* text = canvas->CreateChild<Text>(); 
	text->SetText("GUI Text test");

	Text* text2 = canvas->CreateChild<Text>(); 
	text2->SetText("Text id = %d", 0);
	text2->SetColor(Color(0.5f, 0.5f, 0.5f, 1.0f));

	Button* button = canvas->CreateChild<Button>();
	button->SetText("Button");

	Object::Subsystem<RegisteredBox>()->RegisterCanvas(canvas);
}
void GUISample::Update()
{

	Super::Update();
}

void GUISample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(GUISample)