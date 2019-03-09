#include "UISample.h"

void UISample::Init()
{
	Super::Init();
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("UI Sample");

}
void UISample::Start()
{
	Super::Start();
}
void UISample::Update()
{
	Super::Update();
}

void UISample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(UISample)