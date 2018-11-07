#include "AudioSource.h"


namespace Auto3D {


AudioSource::AudioSource(Ambient* ambient)
	: Super(ambient)
	, _uiBuffer(0)
	, _uiSource(0)
	, _iState(0)
{
	// Generate an AL Buffer
	alGenBuffers(1, &_uiBuffer);

	// Load Wave file into OpenAL /
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath("../Resource/sound/SoundTest.wav"), _uiBuffer))
	{
		AutoErrorCout << "Failed to load " 
			<< ALFWaddMediaPath("../Resource/sound/SoundTest.wav") << AutoCoutEnd;
	}
	// Generate a Source to playback the Buffer
	alGenSources(1, &_uiSource);

	// Attach Source to Buffer
	alSourcei(_uiSource, AL_BUFFER, _uiBuffer);
	ALboolean loop = true;


	// Ô´ÉùÒôµÄÎ»ÖÃ.
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };


	alSourcef(_uiSource, AL_PITCH, 1.0f);

	alSourcef(_uiSource, AL_GAIN, 1.0f);

	alSourcefv(_uiSource, AL_POSITION, SourcePos);

	ALfloat SourceVel[] = { 0.0, 0.0, 0.1 };
	alSourcefv(_uiSource, AL_VELOCITY, SourceVel);

	alSourcei(_uiSource, AL_LOOPING, _isLoop);
}


AudioSource::~AudioSource()
{
	alSourceStop(_uiSource);
	alDeleteSources(1, &_uiSource);
	alDeleteBuffers(1, &_uiBuffer);
}

void AudioSource::Play(double delayTime)
{
	// Play Source
	alSourcePlay(_uiSource);
}

void AudioSource::Pause()
{
	alSourcePause(_uiSource);
}

void AudioSource::Stop(bool enable)
{
	alSourceStop(_uiSource);
}

void AudioSource::SetLoop(bool enable)
{
	_isLoop = enable;
	alSourcei(_uiSource, AL_LOOPING, _isLoop);
}

AudioSourceState AudioSource::GetState()
{
	alGetSourcei(_uiSource, AL_SOURCE_STATE, &_iState);
	if (_iState == AL_INITIAL)
		return AudioSourceState::INITIAL;
	else if (_iState == AL_PLAYING)
		return AudioSourceState::PLAYING;
	else if (_iState == AL_PAUSED)
		return AudioSourceState::PAUSED;
	else if (_iState == AL_STOPPED)
		return AudioSourceState::STOPPED;
	else
		return AudioSourceState::DEFAULT;
}

void AudioSource::attachBuffer(const AudioClip& clip)
{

}

void AudioSource::attachBuffer(AudioClip* clip)
{

}



}