#include "AudioListener.h"
#include "AutoOAL.h"

// Listener position
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

// Listener speed
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

// Listener direction(first 3 elements are "at", second 3 are "up")
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

namespace Auto3D {

AudioListener::AudioListener(Ambient* ambient)
	:Super(ambient)
{
	alListenerfv(AL_POSITION, ListenerPos);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}


AudioListener::~AudioListener()
{
}
void AudioListener::SetListenervalues()
{
	alListenerfv(AL_POSITION, ListenerPos);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

}