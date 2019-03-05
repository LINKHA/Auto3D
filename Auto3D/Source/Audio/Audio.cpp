#include "Audio.h"
#include "../Time/Time.h"
#include "../Time/Timer.h"
#include "../Debug/Log.h"

#include "Sound.h"
#include "AudioListener.h"
#include "AudioSource.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <functional>



#include "../Debug/DebugNew.h"
namespace Auto3D {

const ALCint contextAttr[] = { ALC_FREQUENCY,48000,0 };

Audio::Audio()
{
	_device = alcOpenDevice(NULL);
	_context = alcCreateContext(_device, contextAttr);
	alcMakeContextCurrent(_context);

	RegisterSubsystem(this);
}


Audio::~Audio()
{
	alcDestroyContext(_context);
	alcCloseDevice(_device);

	RemoveSubsystem(this);
}

void Audio::SetListener(AudioListener* listener)
{
	_listener = listener;
}

void Audio::SetListenerValue(Vector3F position, Vector3F listenerVel, Vector3F listenerOriAt, Vector3F listenerOriUp)
{
	if (!_listener)
		return;
	// Listener position
	ALfloat listenerVelArray[] = { listenerVel._x, listenerVel._y, listenerVel._z };

	// Listener speed
	ALfloat ListenerOriArray[] = {
		listenerOriAt._x, listenerOriAt._y, listenerOriAt._z,
		listenerOriUp._x , listenerOriUp._y ,listenerOriUp._z };

	alListener3f(AL_POSITION, position._x, position._y, position._z);
	alListenerfv(AL_VELOCITY, listenerVelArray);
	alListenerfv(AL_ORIENTATION, ListenerOriArray);
}

void Audio::SourcePlay(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid play failed");
		return;
	}
		
	if (!delay)
		alSourcePlay(source);
	else
		Subsystem<Time>()->OneShotTimer(std::bind(&This::SourcePlay, this, source, 0), delay);
}

void Audio::SourcePause(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid pause failed");
		return;
	}
	if (!delay)
		alSourcePause(source);
	else
		Subsystem<Time>()->OneShotTimer(std::bind(&This::SourcePause, this, source, 0), delay);
}

void Audio::SourceStop(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid stop failed");
		return;
	}
	if (!delay)
		alSourceStop(source);
	else
		Subsystem<Time>()->OneShotTimer(std::bind(&This::SourceStop, this, source, 0), delay);
}

void Audio::SourceRewind(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid rewind failed");
		return;
	}
	if (!delay)
		alSourceRewind(source);
	else
		Subsystem<Time>()->OneShotTimer(std::bind(&This::SourceRewind, this, source, 0), delay);
}
void Audio::UpdateAudioData()
{

}


void RegisterAudioLibrary()
{
	static bool registered = false;
	if (registered)
		return;
	registered = true;

	AudioListener::RegisterObject();
	AudioSource::RegisterObject();
	Sound::RegisterObject();
	AudioNode::RegisterObject();

}

}