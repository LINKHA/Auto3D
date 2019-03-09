#pragma once

#include "../Sample.h"

using namespace Auto3D;

class SpriteSample : public Sample
{
	REGISTER_OBJECT_CLASS(SpriteSample, Sample)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
	void HandleCloseRequest(Event& /* event */)
	{
		Subsystem<Graphics>()->Close();
	}
	float yaw = 0.0f, pitch = 0.0f;
	Camera* camera;
	SharedPtr<Scene> scene;
	Vector<Sprite*> sprites;
};