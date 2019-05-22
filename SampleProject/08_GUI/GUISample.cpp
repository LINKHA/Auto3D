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