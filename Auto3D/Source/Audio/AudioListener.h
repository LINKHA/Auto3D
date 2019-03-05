#pragma once
#include "../Scene/SpatialNode.h"

namespace Auto3D
{

class Audio;

class AUTO_API AudioListener : public SpatialNode
{
	REGISTER_OBJECT_CLASS(AudioListener, SpatialNode)

public:
	AudioListener();
	~AudioListener();


	/// Register factory and attributes.
	static void RegisterObject();

	void Update();

private:
	WeakPtr<Audio> _audio;
	//Listener speed
	Vector3F _listenerVel;
	// Listener direction elements are at
	Vector3F _listenerOriAt;
	// Listener direction elements are up
	Vector3F _listenerOriUp;
};

}