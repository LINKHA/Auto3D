#pragma once
#include "AudioNode.h"

namespace Auto3D
{

class AAudio;

class AUTO_API AAudioListener : public AAudioNode
{
	REGISTER_OBJECT_CLASS(AAudioListener, AAudioNode)

public:
	AAudioListener();
	~AAudioListener();

	/// Register factory and attributes.
	static void RegisterObject();

	const Vector3F GetVel()const { return _listenerVel; }
	const Vector3F GetOriAt()const { return _listenerOriAt; }
	const Vector3F GetOriUp()const { return _listenerOriUp; }
private:
	//Listener speed
	Vector3F _listenerVel;
	// Listener direction elements are at
	Vector3F _listenerOriAt;
	// Listener direction elements are up
	Vector3F _listenerOriUp;
};

}