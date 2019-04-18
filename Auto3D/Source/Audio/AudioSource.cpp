#include "AudioSource.h"
#include "Sound.h"
#include "Audio.h"
#include "../Debug/Log.h"
#include "AudioBuffer.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "../Debug/DebugNew.h"


namespace Auto3D 
{

AudioSource::AudioSource() :
	_pitch(1.0f),
	_gain(1.0f),
	_vel(0.0f, 0.0f, 0.1f)
{
	_audio = Object::Subsystem<Audio>();
}

AudioSource::~AudioSource()
{
}

void AudioSource::RegisterObject()
{
	RegisterFactory<AudioSource>();
}

void AudioSource::Play(int delayTime)
{
	if (!_audio)
		WarinningString("Miss audio, can'y play this source.");
	_audio->SourcePlay(_buffer->Source(), delayTime);
}

void AudioSource::Pause(int delayTime)
{
	if (!_audio)
		WarinningString("Miss audio, can'y pause this source.");
	_audio->SourcePause(_buffer->Source(), delayTime);
}

void AudioSource::Stop(int delayTime)
{
	if (!_audio)
		WarinningString("Miss audio, can'y stop this source.");
	_audio->SourceStop(_buffer->Source(), delayTime);
}

void AudioSource::Rewind(int delayTime)
{
	if (!_audio)
		WarinningString("Miss audio, can'y rewind this source.");
	_audio->SourceRewind(_buffer->Source(), delayTime);
}

AudioSourceState::Type AudioSource::GetState()
{
	return _audio->GetState(GetBuffer()->Source());
}


void AudioSource::SetSound(Sound* sound)
{
	_sound = sound;
	_buffer = new AudioBuffer();
	_buffer->Create(sound);
	_audio->AddSource(GetBuffer()->Source(), this);

	_audio->SetPitch(GetBuffer()->Source(), _pitch);
	_audio->SetGain(GetBuffer()->Source(), _gain);
	_audio->SetVel(GetBuffer()->Source(), _vel);
}

}
