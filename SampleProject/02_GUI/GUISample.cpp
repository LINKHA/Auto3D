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

	checkBox = canvas->CreateChild<CheckBox>();
	checkBox->SetText("CheckBox");



	canvas2 = new Canvas();
	canvas2->SetTitle("Add window");
	canvas2->SetEnabled(false);

	Text* text3 = canvas2->CreateChild<Text>();
	text3->SetText("Hello window");
}
void GUISample::Update()
{
	Super::Update();
	if (button->IsClick())
	{
		count++;
		text2->SetText(" Button count = %d", count);
	}
	if (checkBox->IsSelect())
		canvas2->SetEnabled(true);
	else
		canvas2->SetEnabled(false);
}

void GUISample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(GUISample)