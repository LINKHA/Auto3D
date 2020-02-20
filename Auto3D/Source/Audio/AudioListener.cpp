#include "Audio/AudioListener.h"
#include "Audio/Audio.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace Auto3D
{

AAudioListener::AAudioListener() :
	_listenerVel(0.0f, 0.0f, 0.0f),
	_listenerOriAt(0.0f, 0.0f, -1.0f),
	_listenerOriUp(0.0f, 1.0f, 0.0f)
{
	GAudioModule::Get().SetListener(this);

}
AAudioListener::~AAudioListener()
{

}

}