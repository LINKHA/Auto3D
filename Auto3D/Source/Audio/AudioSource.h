#pragma once
#include "AudioNode.h"


namespace Auto3D 
{

class Sound;
class Audio;

struct __AudioSourceState
{
	enum _AudioSourceState
	{
		Default,
		Initial,
		Playing,
		Paused,
		Stopped
	};
};
using AudioSourceState = __AudioSourceState::_AudioSourceState;

class AUTO_API AudioSource : public AudioNode
{

	REGISTER_OBJECT_CLASS(AudioSource, AudioNode)

public:
	/**
	* @brief : The constructor
	*/
	AudioSource();

	~AudioSource();
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject();
	void Start();
	void Update();
	/**
	* @brief : Plays the active audioclip at (future) scheduled time. If time < 0 it specifies a delay
	*/
	void Play(int delayTime = 0);
	/**
	* @brief : Pauses the active audioclip
	*/
	void Pause(int delayTime = 0);
	/**
	* @brief : Stops the active audio clip
	*/
	void Stop(int delayTime = 0);
	/**
	* @brief : Rewind audio clip
	*/
	void Rewind(int delayTime = 0);
	/**
	* @brief : Set audio loop
	*/
	void SetLoop(bool enable);
	/**
	* @brief : Set buffer
	*/
	void SetAudioBuffer(Sound* audioBuffer);
	/**
	* @brief : Get audio source state with AudioSourceState
	*/
	AudioSourceState GetState();
	/**
	* @brief : Attach buffer for point
	*/
	void AttachBuffer(Sound* clip);
private:
	/**
	* @brief : Attach buffer for point
	*/
	void attachBuffer();
private:
	
	/// is playing in this audio source
	bool _isPlaying{};
	///	is pause in this audio source
	bool _isPaused{};
	/// is stop in this audio source
	bool _isStop{};
	/// is loop with this audio source
	bool _isLoop{};
	/// autio source buffer
	unsigned _buffer{};
	/// autio source
	unsigned _source{};
	/// autio source state
	int _state{};
	/// audio buffer
	SharedPtr<Sound> _audioBuffer;
};

}