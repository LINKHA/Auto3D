#include "Audio.h"
#include "Time/Time.h"
#include "Time/Timer.h"
#include "Debug/Log.h"
#include "Core/Modules/ModuleManager.h"

#include "Engine/Components/Sound.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "AudioBuffer.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <functional>



#include "Debug/DebugNew.h"
namespace Auto3D {

const ALCint contextAttr[] = { ALC_FREQUENCY,48000,0 };

FAudioModule::FAudioModule() :
	_device(nullptr),
	_context(nullptr)
{
	_device = alcOpenDevice(NULL);
	_context = alcCreateContext(_device, contextAttr);
	alcMakeContextCurrent(_context);
	// The application has been placed in the AAudioListener
	//RegisterModule(this);
}


FAudioModule::~FAudioModule()
{
	alcDestroyContext(_context);
	alcCloseDevice(_device);
}

void FAudioModule::AddSource(unsigned sourceID, AAudioSource* source)
{
	if (_sources.Find(sourceID) != _sources.End())
		return;
	_sources[sourceID] = source;
}

void FAudioModule::SetListener(AAudioListener* listener)
{
	_listener = listener;
}

void FAudioModule::SetListenerValue(TVector3F position, TVector3F listenerVel, TVector3F listenerOriAt, TVector3F listenerOriUp)
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

void FAudioModule::SourcePlay(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid play failed");
		return;
	}
	CallSourcePlay(source, delay);
}

void FAudioModule::SourcePlay(AAudioSource* source, int delay)
{
	if (!source)
	{
		ErrorString("AudioSource invalid play failed");
		return;
	}	
	if (!source->GetBuffer()->Source())
	{
		ErrorString("Audio source invalid play failed");
		return;
	}
	CallSourcePlay(source->GetBuffer()->Source(), delay);
}

void FAudioModule::SourcePause(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid pause failed");
		return;
	}
	CallSourcePause(source, delay);
}

void FAudioModule::SourcePause(AAudioSource* source, int delay)
{
	if (!source)
	{
		ErrorString("AudioSource invalid pause failed");
		return;
	}
	if (!source->GetBuffer()->Source())
	{
		ErrorString("Audio source invalid pause failed");
		return;
	}
	CallSourcePause(source->GetBuffer()->Source(), delay);
}

void FAudioModule::SourceStop(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid stop failed");
		return;
	}
	CallSourceStop(source, delay);
}

void FAudioModule::SourceStop(AAudioSource* source, int delay)
{
	if (!source)
	{
		ErrorString("AudioSource invalid stop failed");
		return;
	}
	if (!source->GetBuffer()->Source())
	{
		ErrorString("Audio source invalid stop failed");
		return;
	}
	CallSourceStop(source->GetBuffer()->Source(), delay);
}

void FAudioModule::SourceRewind(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid rewind failed");
		return;
	}
	CallSourceRewind(source, delay);
}

void FAudioModule::SourceRewind(AAudioSource* source, int delay)
{
	if (!source)
	{
		ErrorString("AudioSource invalid rewind failed");
		return;
	}
	if (!source->GetBuffer()->Source())
	{
		ErrorString("Audio source invalid rewind failed");
		return;
	}
	CallSourceRewind(source->GetBuffer()->Source(), delay);
}

void FAudioModule::SetPitch(unsigned source, float val)
{
	if(_sources.Find(source) != _sources.End())
		alSourcef(source, AL_PITCH, val);
}

void FAudioModule::SetGain(unsigned source, float val)
{
	if (_sources.Find(source) != _sources.End())
		alSourcef(source, AL_GAIN, 1.0f);
}

void FAudioModule::SetVel(unsigned source, TVector3F vel)
{
	if (_sources.Find(source) != _sources.End())
		alSource3f(source, AL_VELOCITY, vel._x, vel._y, vel._z);
}

EAudioSourceState::Type FAudioModule::GetState(unsigned source)
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	if (state == AL_INITIAL)
		return EAudioSourceState::INITIAL;
	else if (state == AL_PLAYING)
		return EAudioSourceState::PLAYING;
	else if (state == AL_PAUSED)
		return EAudioSourceState::PAUSED;
	else if (state == AL_STOPPED)
		return EAudioSourceState::STOPPED;
	else
		return EAudioSourceState::DEFAULT;
}
EAudioSourceState::Type FAudioModule::GetState(AAudioSource* source)
{
	if (!source)
	{
		ErrorString("AudioSource invalid get state failed");
		return EAudioSourceState::DEFAULT;
	}
	if (!source->GetBuffer()->Source())
	{
		ErrorString("Audio source invalid get state failed");
		return EAudioSourceState::DEFAULT;
	}
	ALint state;
	alGetSourcei(source->GetBuffer()->Source(), AL_SOURCE_STATE, &state);
	if (state == AL_INITIAL)
		return EAudioSourceState::INITIAL;
	else if (state == AL_PLAYING)
		return EAudioSourceState::PLAYING;
	else if (state == AL_PAUSED)
		return EAudioSourceState::PAUSED;
	else if (state == AL_STOPPED)
		return EAudioSourceState::STOPPED;
	else
		return EAudioSourceState::DEFAULT;
}
const AAudioSource* FAudioModule::GetSource(unsigned index)
{
	return _sources.Find(index) != _sources.End() ? _sources[index] : nullptr;
}

void FAudioModule::Update()
{
	if (!_listener)
		return;

	TVector3F vec = _listener->GetPosition();
	TVector3F ListenerVel = _listener->GetVel();
	TVector3F listenerOriAt = _listener->GetOriAt();
	TVector3F listenerOriUp = _listener->GetOriUp();

	// Listener speed
	ALfloat ListenerOriArray[] = {
		listenerOriAt._x, listenerOriAt._y, listenerOriAt._z,
		listenerOriUp._x , listenerOriUp._y ,listenerOriUp._z };

	alListener3f(AL_POSITION, vec._x, vec._y, vec._z);
	alListener3f(AL_VELOCITY, ListenerVel._x, ListenerVel._y, ListenerVel._z);
	alListenerfv(AL_ORIENTATION, ListenerOriArray);

	for (auto it = _sources.Begin(); it != _sources.End(); it++)
	{
		AAudioSource& source = *it->_second;
		TVector3F vec = (*it)._second->GetPosition();
		alSource3f((*it)._second->GetBuffer()->Source(), AL_POSITION, vec._x, vec._y, vec._z);
	}

}
bool FAudioModule::IsInitialized()
{
	return _device && _context;
}

void FAudioModule::CallSourcePlay(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid play failed");
		return;
	}

	if (!delay)
		alSourcePlay(source);
	else
		GModuleManager::Get().TimeModule()->OneShotTimer(std::bind(&FAudioModule::CallSourcePlay, this, source, 0), delay);
}

void FAudioModule::CallSourcePause(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid pause failed");
		return;
	}
	if (!delay)
		alSourcePause(source);
	else
		GModuleManager::Get().TimeModule()->OneShotTimer(std::bind(&FAudioModule::CallSourcePause, this, source, 0), delay);
}

void FAudioModule::CallSourceStop(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid stop failed");
		return;
	}
	if (!delay)
		alSourceStop(source);
	else
		GModuleManager::Get().TimeModule()->OneShotTimer(std::bind(&FAudioModule::CallSourceStop, this, source, 0), delay);
}

void FAudioModule::CallSourceRewind(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid rewind failed");
		return;
	}
	if (!delay)
		alSourceRewind(source);
	else
		GModuleManager::Get().TimeModule()->OneShotTimer(std::bind(&FAudioModule::CallSourceRewind, this, source, 0), delay);

}

}