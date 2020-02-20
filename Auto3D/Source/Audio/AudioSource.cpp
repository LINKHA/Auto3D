#include "AudioSource.h"
#include "Resource/Sound.h"
#include "Audio.h"
#include "Debug/Log.h"
#include "AudioBuffer.h"
#include "Resource/ResourceCache.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "Debug/DebugNew.h"


namespace Auto3D 
{


AAudioSource::AAudioSource() :
	_pitch(1.0f),
	_gain(1.0f),
	_vel(0.0f, 0.0f, 0.1f)
{
}

AAudioSource::~AAudioSource()
{
}

void AAudioSource::Play(int delayTime)
{
	GAudioModule::Get().SourcePlay(_buffer->Source(), delayTime);
}

void AAudioSource::Pause(int delayTime)
{
	GAudioModule::Get().SourcePause(_buffer->Source(), delayTime);
}

void AAudioSource::Stop(int delayTime)
{
	GAudioModule::Get().SourceStop(_buffer->Source(), delayTime);
}

void AAudioSource::Rewind(int delayTime)
{
	GAudioModule::Get().SourceRewind(_buffer->Source(), delayTime);
}

EAudioSourceState::Type AAudioSource::GetState()
{
	return GAudioModule::Get().GetState(GetBuffer()->Source());
}


void AAudioSource::SetSound(OSound* sound)
{
	_sound = sound;
	_buffer = new FAudioBuffer();
	_buffer->Create(sound);
	GAudioModule::Get().AddSource(GetBuffer()->Source(), this);

	GAudioModule::Get().SetPitch(GetBuffer()->Source(), _pitch);
	GAudioModule::Get().SetGain(GetBuffer()->Source(), _gain);
	GAudioModule::Get().SetVel(GetBuffer()->Source(), _vel);
}

void AAudioSource::SetSoundAttr(FResourceRef sound)
{
	GResourceModule& cache = GResourceModule::Get();
	SetSound(cache.LoadResource<OSound>(sound._pathName.CString()));
}

FResourceRef AAudioSource::GetSoundAttr() const
{
	return FResourceRef(AAudioSource::GetTypeNameStatic(), ResourceName(_sound));
}
}
