#include "AutoEditor.h"

void AutoEditor::Init()
{
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("AutoEditor");
}

void AutoEditor::Start()
{
	
}
void AutoEditor::Update()
{

	
}

void AutoEditor::Stop()
{
	
}

AUTO_APPLICATION_MAIN(AutoEditor)