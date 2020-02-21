#include "Component/AudioSourceComponent.h"
#include "Resource/Sound.h"
#include "Audio/Audio.h"
#include "Debug/Log.h"
#include "Audio/AudioBuffer.h"
#include "Resource/ResourceCache.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "Debug/DebugNew.h"


namespace Auto3D 
{


AAudioSourceComponent::AAudioSourceComponent() :
	_pitch(1.0f),
	_gain(1.0f),
	_vel(0.0f, 0.0f, 0.1f)
{
}

AAudioSourceComponent::~AAudioSourceComponent()
{
}

void AAudioSourceComponent::Play(int delayTime)
{
	GAudioModule::Get().SourcePlay(_buffer->Source(), delayTime);
}

void AAudioSourceComponent::Pause(int delayTime)
{
	GAudioModule::Get().SourcePause(_buffer->Source(), delayTime);
}

void AAudioSourceComponent::Stop(int delayTime)
{
	GAudioModule::Get().SourceStop(_buffer->Source(), delayTime);
}

void AAudioSourceComponent::Rewind(int delayTime)
{
	GAudioModule::Get().SourceRewind(_buffer->Source(), delayTime);
}

EAudioSourceState::Type AAudioSourceComponent::GetState()
{
	return GAudioModule::Get().GetState(GetBuffer()->Source());
}


void AAudioSourceComponent::SetSound(OSound* sound)
{
	_sound = sound;
	_buffer = new FAudioBuffer();
	_buffer->Create(sound);
	GAudioModule::Get().AddSource(GetBuffer()->Source(), this);

	GAudioModule::Get().SetPitch(GetBuffer()->Source(), _pitch);
	GAudioModule::Get().SetGain(GetBuffer()->Source(), _gain);
	GAudioModule::Get().SetVel(GetBuffer()->Source(), _vel);
}

void AAudioSourceComponent::SetSoundAttr(FResourceRef sound)
{
	GResourceModule& cache = GResourceModule::Get();
	SetSound(cache.LoadResource<OSound>(sound._pathName.CString()));
}

FResourceRef AAudioSourceComponent::GetSoundAttr() const
{
	return FResourceRef(AAudioSourceComponent::GetTypeNameStatic(), ResourceName(_sound));
}
}
