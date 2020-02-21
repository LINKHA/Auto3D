#include "Audio.h"
#include "Time/Time.h"
#include "Time/Timer.h"
#include "Debug/Log.h"

#include "Resource/Sound.h"
#include "Component/AudioListenerComponent.h"
#include "Component/AudioSourceComponent.h"
#include "Audio/AudioBuffer.h"
#include "Component/TransformComponent.h"
#include "Gameplay/Actor.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <functional>



#include "Debug/DebugNew.h"
namespace Auto3D {

const ALCint contextAttr[] = { ALC_FREQUENCY,48000,0 };

IMPLEMENT_SINGLETON(GAudioModule)

GAudioModule::GAudioModule() :
	_device(nullptr),
	_context(nullptr)
{
	_device = alcOpenDevice(NULL);
	_context = alcCreateContext(_device, contextAttr);
	alcMakeContextCurrent(_context);
	// The application has been placed in the AAudioListenerComponent
	//RegisterModule(this);
}


GAudioModule::~GAudioModule()
{
	alcDestroyContext(_context);
	alcCloseDevice(_device);
}

void GAudioModule::AddSource(unsigned sourceID, AAudioSourceComponent* source)
{
	if (_sources.Find(sourceID) != _sources.End())
		return;
	_sources[sourceID] = source;
}

void GAudioModule::SetListener(AAudioListenerComponent* listener)
{
	_listener = listener;
}

void GAudioModule::SetListenerValue(TVector3F position, TVector3F listenerVel, TVector3F listenerOriAt, TVector3F listenerOriUp)
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

void GAudioModule::SourcePlay(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid play failed");
		return;
	}
	CallSourcePlay(source, delay);
}

void GAudioModule::SourcePlay(AAudioSourceComponent* source, int delay)
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

void GAudioModule::SourcePause(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid pause failed");
		return;
	}
	CallSourcePause(source, delay);
}

void GAudioModule::SourcePause(AAudioSourceComponent* source, int delay)
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

void GAudioModule::SourceStop(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid stop failed");
		return;
	}
	CallSourceStop(source, delay);
}

void GAudioModule::SourceStop(AAudioSourceComponent* source, int delay)
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

void GAudioModule::SourceRewind(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid rewind failed");
		return;
	}
	CallSourceRewind(source, delay);
}

void GAudioModule::SourceRewind(AAudioSourceComponent* source, int delay)
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

void GAudioModule::SetPitch(unsigned source, float val)
{
	if(_sources.Find(source) != _sources.End())
		alSourcef(source, AL_PITCH, val);
}

void GAudioModule::SetGain(unsigned source, float val)
{
	if (_sources.Find(source) != _sources.End())
		alSourcef(source, AL_GAIN, 1.0f);
}

void GAudioModule::SetVel(unsigned source, TVector3F vel)
{
	if (_sources.Find(source) != _sources.End())
		alSource3f(source, AL_VELOCITY, vel._x, vel._y, vel._z);
}

EAudioSourceState::Type GAudioModule::GetState(unsigned source)
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
EAudioSourceState::Type GAudioModule::GetState(AAudioSourceComponent* source)
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
const AAudioSourceComponent* GAudioModule::GetSource(unsigned index)
{
	return _sources.Find(index) != _sources.End() ? _sources[index] : nullptr;
}

void GAudioModule::Update()
{
	if (!_listener)
		return;

	TVector3F vec = _listener->GetOwner()->GetTransform()->GetWorldPosition();
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
		AAudioSourceComponent& source = *it->_second;
		TVector3F vec = (*it)._second->GetOwner()->GetTransform()->GetWorldPosition();
		alSource3f((*it)._second->GetBuffer()->Source(), AL_POSITION, vec._x, vec._y, vec._z);
	}

}
bool GAudioModule::IsInitialized()
{
	return _device && _context;
}

void GAudioModule::CallSourcePlay(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid play failed");
		return;
	}

	if (!delay)
		alSourcePlay(source);
	else
		GTimeModule::Get().ShotTimer(std::bind(&GAudioModule::CallSourcePlay, this, source, 0), delay);
}

void GAudioModule::CallSourcePause(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid pause failed");
		return;
	}
	if (!delay)
		alSourcePause(source);
	else
		GTimeModule::Get().ShotTimer(std::bind(&GAudioModule::CallSourcePause, this, source, 0), delay);
}

void GAudioModule::CallSourceStop(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid stop failed");
		return;
	}
	if (!delay)
		alSourceStop(source);
	else
		GTimeModule::Get().ShotTimer(std::bind(&GAudioModule::CallSourceStop, this, source, 0), delay);
}

void GAudioModule::CallSourceRewind(unsigned source, int delay)
{
	if (!source)
	{
		ErrorString("Audio source invalid rewind failed");
		return;
	}
	if (!delay)
		alSourceRewind(source);
	else
		GTimeModule::Get().ShotTimer(std::bind(&GAudioModule::CallSourceRewind, this, source, 0), delay);

}

}