#include "AudioListener.h"
#include "AutoOAL.h"
#include "Ambient.h"


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
}

AudioListener::~AudioListener()
{
}

void AudioListener::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<AudioListener>(SCENE_ATTACH);
}


void AudioListener::Start()
{
	Vector3 vec = GetPosition();
	alListener3f(AL_POSITION, vec.x, vec.y, vec.z);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}
void AudioListener::Update()
{
	Vector3 vec = GetPosition();
	alListener3f(AL_POSITION, vec.x, vec.y, vec.z);
	alListenerfv(AL_VELOCITY, ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

}