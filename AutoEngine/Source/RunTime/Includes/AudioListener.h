#pragma once
#include "Component.h"

namespace Auto3D {

class AudioListener : public Component
{
	REGISTER_OBJECT_CLASS(AudioListener, Component)
public:
	explicit AudioListener(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);

	void Start()override;
	void Update()override;
};

}