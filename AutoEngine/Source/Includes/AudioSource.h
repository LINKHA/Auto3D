#pragma once
#include "Component.h"
#include "AutoOAL.h"

namespace Auto3D {
class AudioBuffer;

enum class AudioSourceState
{
	DEFAULT,
	INITIAL,
	PLAYING,
	PAUSED,
	STOPPED
};

class AudioSource : public Component
{
	REGISTER_OBJECT_CLASS(AudioSource, Component)
public:
	/**
	* @brief : The constructor
	*/
	explicit AudioSource(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Start component
	*/
	void Start()override;
	/**
	* @brief : Update component
	*/
	void Update()override;
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
	void SetAudioBuffer(SharedPtr<AudioBuffer> audioBuffer);
	/**
	* @brief : Is the audio source currently playing
	*/
	bool IsPlaying() const { return _isPlaying; }
	/**
	* @brief : Is the audio source currently paused
	*/
	bool IsPaused() const { return _isPaused; }
	/**
	* @brief : Is the audio source currently stop
	*/
	bool IsStop() const { return _isStop; }
	/**
	* @brief : Get audio source state with AudioSourceState
	*/
	AudioSourceState GetState();
	/**
	* @brief : Attach buffer for point
	*/
	void AttachBuffer(SharedPtr<AudioBuffer> clip);
private:
	/**
	* @brief : Attach buffer for point
	*/
	void attachBuffer();
	/**
	* @brief : Play audio
	*/
	void callPlay();
	/**
	* @brief : Pause audio
	*/
	void callPause();
	/**
	* @brief : Stop audio
	*/
	void callStop();
	/**
	* @brief : Rewind audio
	*/
	void callRewind();
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
	ALuint _buffer{};
	/// autio source
	ALuint _source{};
	/// autio source state
	ALint _state{};
	/// audio buffer
	SharedPtr<AudioBuffer> _audioBuffer;
};

}