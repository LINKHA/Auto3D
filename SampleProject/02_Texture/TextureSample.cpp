#include "TextureSample.h"

void TextureSample::Init()
{

}
void TextureSample::Start()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	auto* graphics = Object::Subsystem<Graphics>();

	SubscribeToEvent(graphics->RenderWindow()->closeRequestEvent, &TextureSample::HandleCloseRequest);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	//camera->SetPosition(Vector3F(0.0f, 20.0f, -75.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));
	// Register scene to scene system use to render
	Object::Subsystem<SceneSystem>()->RegisterScene(scene, camera);

}
void TextureSample::Update()
{
	auto graphics = Subsystem<Graphics>();

	auto* input = Object::Subsystem<Input>();
	pitch += input->MouseMove()._y * 0.25f;
	yaw += input->MouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	camera->SetRotation(Quaternion(pitch, yaw, 0.0f));

	// Update camera aspect ratio based on window size
	camera->SetAspectRatio((float)graphics->Width() / (float)graphics->Height());

}

void TextureSample::Stop()
{
}

AUTO_APPLICATION_MAIN(TextureSample)