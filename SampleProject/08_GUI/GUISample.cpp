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

	text = canvas->CreateChild<Text>(); 
	text->SetText("GUI Text test");

	button = canvas->CreateChild<Button>();
	button->SetText("Button");

	text2 = canvas->CreateChild<Text>();
	text2->SetText(" Button count = %d", count);
	text2->SetColor(Color(1.0f, 0.5f, 0.0f, 1.0f));
	text2->SetSameLine();

	Slider* slider = canvas->CreateChild<Slider>();
	slider->SetText("Slider");
	slider->SetRange(0.0f, 100.0f);

	ColorEdit* colorEdit = canvas->CreateChild<ColorEdit>();

	CheckBox* checkBox = canvas->CreateChild<CheckBox>();
	checkBox->SetText("CheckBox");

	Object::Subsystem<RegisteredBox>()->RegisterCanvas(canvas);
}
void GUISample::Update()
{
	Super::Update();
	if (button->IsClick())
	{
		count++;
		text2->SetText(" Button count = %d", count);
	}
}

void GUISample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(GUISample)