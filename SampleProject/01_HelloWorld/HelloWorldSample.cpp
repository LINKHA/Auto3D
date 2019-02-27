#include "HelloWorldSample.h"

void HelloWorldSample::Init()
{

}
void HelloWorldSample::Start()
{
	auto* cache = Object::GetSubsystem<ResourceCache>();
	auto* graphics = Object::GetSubsystem<Graphics>();

	SubscribeToEvent(graphics->RenderWindow()->closeRequestEvent, &HelloWorldSample::HandleCloseRequest);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	//camera->SetPosition(Vector3F(0.0f, 20.0f, -75.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));
	// Register scene to scene system use to render
	Object::GetSubsystem<SceneSystem>()->RegisterScene(scene, camera);

}
void HelloWorldSample::Update()
{
	auto graphics = GetSubsystem<Graphics>();

	auto* input = Object::GetSubsystem<Input>();
	pitch += input->MouseMove()._y * 0.25f;
	yaw += input->MouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	camera->SetRotation(Quaternion(pitch, yaw, 0.0f));

	// Update camera aspect ratio based on window size
	camera->SetAspectRatio((float)graphics->Width() / (float)graphics->Height());

}

void HelloWorldSample::Stop()
{
}

AUTO_APPLICATION_MAIN(HelloWorldSample)