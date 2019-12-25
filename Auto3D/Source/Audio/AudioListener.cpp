#include "AudioListener.h"
#include "Audio.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace Auto3D
{
// Font cpp fail,Initializes the global constructor so apply here
// Apply in AudioNode CPP
//REGISTER_CLASS
//{
//	REGISTER_CALSS_FACTORY_IMP(AAudioListener)
//	.constructor<>()
//	.property("vel", &AAudioListener::GetVel, &AAudioListener::SetVel)
//	(
//		metadata(SERIALIZABLE, "")
//	)
//	.property("oriAt", &AAudioListener::GetOriAt, &AAudioListener::SetOriAt)
//	(
//		metadata(SERIALIZABLE, "")
//	)
//	.property("oriUp", &AAudioListener::GetOriUp, &AAudioListener::SetOriUp)
//	(
//		metadata(SERIALIZABLE, "")
//	)
//	;
//}

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

}