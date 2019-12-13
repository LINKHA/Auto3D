#include "AudioListener.h"
#include "Audio.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace Auto3D
{

REGISTER_CLASS
{
	using namespace rttr;
	registration::class_<AAudioListener>("AudioListener")
	.constructor<>()
	.property("vel", &AAudioListener::GetVel, &AAudioListener::SetVel)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("oriAt", &AAudioListener::GetOriAt, &AAudioListener::SetOriAt)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("oriUp", &AAudioListener::GetOriUp, &AAudioListener::SetOriUp)
	(
		metadata(SERIALIZABLE, "")
	)
	;
}

AAudioListener::AAudioListener() :
	_listenerVel(0.0f, 0.0f, 0.0f),
	_listenerOriAt(0.0f, 0.0f, -1.0f),
	_listenerOriUp(0.0f, 1.0f, 0.0f)
{
	_audio->SetListener(this);

}
AAudioListener::~AAudioListener()
{

}

void AAudioListener::RegisterObject()
{
	RegisterFactory<AAudioListener>();
}



}