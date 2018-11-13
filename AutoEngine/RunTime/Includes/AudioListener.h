#pragma once
#include "Component.h"

namespace Auto3D {

class AudioListener : public Component
{
	REGISTER_DERIVED_CLASS(AudioListener, Component);
	DECLARE_OBJECT_SERIALIZE(AudioListener);
public:
	explicit AudioListener(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

	void Start()override;
	void Update()override;
};

}