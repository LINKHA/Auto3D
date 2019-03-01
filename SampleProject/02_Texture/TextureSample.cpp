#include "TextureSample.h"

void TextureSample::Init()
{

}
void TextureSample::Start()
{
	SubscribeToEvent(Subsystem<Graphics>()->RenderWindow()->closeRequestEvent, &TextureSample::HandleCloseRequest);
	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 5.0f, -10.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));
	Subsystem<SceneSystem>()->RegisterScene(scene, camera);
}
void TextureSample::Update()
{

}

void TextureSample::Stop()
{
}

AUTO_APPLICATION_MAIN(TextureSample)