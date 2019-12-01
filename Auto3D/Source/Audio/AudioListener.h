#pragma once
#include "AudioNode.h"

namespace Auto3D
{

class AAudio;

class AUTO_API AAudioListener : public AAudioNode
{
	REGISTER_OBJECT(AAudioListener, AAudioNode)

public:
	AAudioListener();
	~AAudioListener();

	/// Register factory and attributes.
	static void RegisterObject();

	const TVector3F GetVel()const { return _listenerVel; }
	const TVector3F GetOriAt()const { return _listenerOriAt; }
	const TVector3F GetOriUp()const { return _listenerOriUp; }
private:
	//Listener speed
	TVector3F _listenerVel;
	// Listener direction elements are at
	TVector3F _listenerOriAt;
	// Listener direction elements are up
	TVector3F _listenerOriUp;
};

}