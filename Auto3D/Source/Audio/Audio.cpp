#include "Audio.h"
#include "../Time/Time.h"
#include "../Time/Timer.h"
#include "../Debug/Log.h"

#include "Sound.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "AudioBuffer.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <functional>



#include "../Debug/DebugNew.h"
namespace Auto3D {

const ALCint contextAttr[] = { ALC_FREQUENCY,48000,0 };

Audio::Audio() :
	_device(nullptr),
	_context(nullptr)
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

void Audio::AddSource(unsigned sourceID, AudioSource* source)
{
	if (_sources.Find(sourceID) != _sources.End())
		return;
	_sources[sourceID] = source;
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


void Audio::SetPitch(unsigned source, float val)
{
	if(_sources.Find(source) != _sources.End())
		alSourcef(source, AL_PITCH, val);
}

void Audio::SetGain(unsigned source, float val)
{
	if (_sources.Find(source) != _sources.End())
		alSourcef(source, AL_GAIN, 1.0f);
}

void Audio::SetVel(unsigned source, Vector3F vel)
{
	if (_sources.Find(source) != _sources.End())
		alSource3f(source, AL_VELOCITY, vel._x, vel._y, vel._z);
}

AudioSourceState Audio::GetState(unsigned source)
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	if (state == AL_INITIAL)
		return AudioSourceState::Initial;
	else if (state == AL_PLAYING)
		return AudioSourceState::Playing;
	else if (state == AL_PAUSED)
		return AudioSourceState::Paused;
	else if (state == AL_STOPPED)
		return AudioSourceState::Stopped;
	else
		return AudioSourceState::Default;
}

const AudioSource* Audio::GetSource(unsigned index)
{
	return _sources.Find(index) != _sources.End() ? _sources[index] : nullptr;
}

void Audio::Update()
{
	Vector3F vec = _listener->GetPosition();
	Vector3F ListenerVel = _listener->GetVel();
	Vector3F listenerOriAt = _listener->GetOriAt();
	Vector3F listenerOriUp = _listener->GetOriUp();

	// Listener speed
	ALfloat ListenerOriArray[] = {
		listenerOriAt._x, listenerOriAt._y, listenerOriAt._z,
		listenerOriUp._x , listenerOriUp._y ,listenerOriUp._z };

	alListener3f(AL_POSITION, vec._x, vec._y, vec._z);
	alListener3f(AL_VELOCITY, ListenerVel._x, ListenerVel._y, ListenerVel._z);
	alListenerfv(AL_ORIENTATION, ListenerOriArray);

	for (auto it = _sources.Begin(); it != _sources.End(); it++)
	{
		AudioSource& source = *it->second;
		Vector3F vec = (*it).second->GetPosition();
		alSource3f((*it).second->GetBuffer()->Source(), AL_POSITION, vec._x, vec._y, vec._z);
	}

}
bool Audio::IsInitialized()
{
	return _device && _context;
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