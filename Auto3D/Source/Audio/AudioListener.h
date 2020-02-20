#pragma once
#include "AudioNode.h"
#include "Math/Vector3.h"

namespace Auto3D
{

class AUTO_API AAudioListener : public AAudioComponent
{
	DECLARE_A_CLASS(AAudioListener, AAudioComponent)
public:
	/// Constructor
	AAudioListener();
	/// Destructor
	~AAudioListener();

public:
	/// Get listener vel.
	const TVector3F& GetVel()const { return _listenerVel; }
	/// Set listener vel.
	void SetVel(const TVector3F& vel) { _listenerVel = vel; }
	/// Get direction elements are at.
	const TVector3F& GetOriAt()const { return _listenerOriAt; }
	/// Set direction elements are at.
	void SetOriAt(const TVector3F& oriAt) { _listenerOriAt = oriAt; }
	/// Get direction elements are up.
	const TVector3F& GetOriUp()const { return _listenerOriUp; }
	/// Set direction elements are up.
	void SetOriUp(const TVector3F& oriUp) { _listenerOriUp = oriUp; }
private:
	//Listener speed.
	TVector3F _listenerVel;
	// Listener direction elements are at.
	TVector3F _listenerOriAt;
	// Listener direction elements are up.
	TVector3F _listenerOriUp;
};

}