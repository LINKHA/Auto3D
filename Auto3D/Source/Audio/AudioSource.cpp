#include "AudioSource.h"
#include "AudioBuffer.h"
#include "Audio.h"
#include "../Debug/Log.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "../Debug/DebugNew.h"


namespace Auto3D 
{

AudioSource::AudioSource()
{
	
	_audio = Object::Subsystem<Audio>();
}

AudioSource::~AudioSource()
{
	alSourceStop(_source);
	alDeleteSources(1, &_source);
	alDeleteBuffers(1, &_buffer);
}


void AudioSource::RegisterObject()
{
	RegisterFactory<AudioSource>();
}

void AudioSource::Update()
{
	Vector3F vec = GetPosition();
	alSource3f(_source, AL_POSITION, vec._x, vec._y, vec._z);
}


void AudioSource::Play(int delayTime)
{
	if (!_audio)
		WarinningString("Miss audio, can'y play this source.");
	_audio->SourcePlay(_source, delayTime);
}

void AudioSource::Pause(int delayTime)
{
	if (!_audio)
		WarinningString("Miss audio, can'y pause this source.");
	_audio->SourcePause(_source, delayTime);
}

void AudioSource::Stop(int delayTime)
{
	if (!_audio)
		WarinningString("Miss audio, can'y stop this source.");
	_audio->SourceStop(_source, delayTime);
}

void AudioSource::Rewind(int delayTime)
{
	if (!_audio)
		WarinningString("Miss audio, can'y rewind this source.");
	_audio->SourceRewind(_source, delayTime);
}

void AudioSource::SetLoop(bool enable)
{
	_isLoop = enable;
	alSourcei(_source, AL_LOOPING, _isLoop);
}

AudioSourceState AudioSource::GetState()
{
	alGetSourcei(_source, AL_SOURCE_STATE, &_state);
	if (_state == AL_INITIAL)
		return AudioSourceState::Initial;
	else if (_state == AL_PLAYING)
		return AudioSourceState::Playing;
	else if (_state == AL_PAUSED)
		return AudioSourceState::Paused;
	else if (_state == AL_STOPPED)
		return AudioSourceState::Stopped;
	else
		return AudioSourceState::Default;
}

void AudioSource::Start()
{

	// Generate an AL Buffer
	alGenBuffers(1, &_buffer);
	alGenSources(1, &_source);

	if (_audioBuffer)
	{
		attachBuffer();
	}

	alSourcei(_source, AL_BUFFER, _buffer);

	alSourcef(_source, AL_PITCH, 1.0f);

	alSourcef(_source, AL_GAIN, 1.0f);

	Vector3F vec = GetPosition();
	alSource3f(_source, AL_POSITION, vec._x, vec._y, vec._z);

	ALfloat SourceVel[] = { 0.0, 0.0, 0.1 };
	alSourcefv(_source, AL_VELOCITY, SourceVel);

	alSourcei(_source, AL_LOOPING, _isLoop);
}

void AudioSource::AttachBuffer(AudioBuffer* clip)
{
	_audioBuffer = clip;
	attachBuffer();
}

void AudioSource::SetAudioBuffer(AudioBuffer* audioBuffer)
{
	_audioBuffer = audioBuffer;
}



void AudioSource::attachBuffer()
{

	long dataSize = 0;

	const ALvoid* data = _audioBuffer->GetData();
	dataSize = _audioBuffer->GetSize();

	/* for simplicity, assume raw file is signed-16b at 44.1kHz */
	alBufferData(_buffer, AL_FORMAT_MONO16, data, dataSize, 44100);

	alSourcei(_source, AL_BUFFER, _buffer);
}

void AudioSource::callPlay()
{
	alSourcePlay(_source);
}

void AudioSource::callPause()
{
	alSourcePause(_source);
}

void AudioSource::callStop()
{
	alSourceStop(_source);
}

void AudioSource::callRewind()
{
	alSourceRewind(_source);
}

}