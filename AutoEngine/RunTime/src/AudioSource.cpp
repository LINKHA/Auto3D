#include "AudioSource.h"
#include "Time.h"
#include "Timer.h"
#include <functional>
namespace Auto3D {

AudioSource::AudioSource(Ambient* ambient)
	: Super(ambient)
{}

AudioSource::AudioSource(Ambient* ambient, AudioBuffer* bufferClip)
	: Super(ambient)
	, _bufferClip(bufferClip)
{}

AudioSource::~AudioSource()
{
	alSourceStop(_source);
	alDeleteSources(1, &_source);
	alDeleteBuffers(1, &_buffer);
}

void AudioSource::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<AudioSource>(SCENE_ATTACH);
}

void AudioSource::Start()
{
	// Generate an AL Buffer
	alGenBuffers(1, &_buffer);
	alGenSources(1, &_source);
	if (_bufferClip) 
	{
		Print(_bufferClip->GetData());
		attachBuffer();
	}
	alSourcei(_source, AL_BUFFER, _buffer);

	alSourcef(_source, AL_PITCH, 1.0f);

	alSourcef(_source, AL_GAIN, 1.0f);

	Vector3 vec = GetPosition();
	alSource3f(_source, AL_POSITION, vec.x, vec.y, vec.z);

	ALfloat SourceVel[] = { 0.0, 0.0, 0.1 };
	alSourcefv(_source, AL_VELOCITY, SourceVel);

	alSourcei(_source, AL_LOOPING, _isLoop);
}

void AudioSource::Update()
{
	Vector3 vec = GetPosition();
	alSource3f(_source, AL_POSITION, vec.x, vec.y, vec.z);
}

void AudioSource::Play(int delayTime)
{
	GetSubSystem<Time>()->OneShotTimer(std::bind(&This::callPlay, this), delayTime);
}

void AudioSource::Pause(int delayTime)
{
	GetSubSystem<Time>()->OneShotTimer(std::bind(&This::callPause, this), delayTime);
}

void AudioSource::Stop(int delayTime)
{
	GetSubSystem<Time>()->OneShotTimer(std::bind(&This::callStop, this), delayTime);
}

void AudioSource::Rewind(int delayTime)
{
	GetSubSystem<Time>()->OneShotTimer(std::bind(&This::callRewind, this), delayTime);
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
		return AudioSourceState::INITIAL;
	else if (_state == AL_PLAYING)
		return AudioSourceState::PLAYING;
	else if (_state == AL_PAUSED)
		return AudioSourceState::PAUSED;
	else if (_state == AL_STOPPED)
		return AudioSourceState::STOPPED;
	else
		return AudioSourceState::DEFAULT;
}

void AudioSource::AttachBuffer(AudioBuffer* clip)
{
	_bufferClip = clip;
	attachBuffer();
}

void AudioSource::attachBuffer()
{
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(_bufferClip->GetData().c_str()), _buffer))
	{
		AutoErrorCout << "Failed to load "
			<< ALFWaddMediaPath(_bufferClip->GetData().c_str()) << AutoCoutEnd;
	}
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