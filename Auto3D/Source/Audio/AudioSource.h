#pragma once
#include "../Scene/SpatialNode.h"


namespace Auto3D {

class AudioBuffer;
class Audio;

class Graphics;

enum class AudioSourceState
{
	Default,
	Initial,
	Playing,
	Paused,
	Stopped
};

class AUTO_API AudioSource : public SpatialNode
{

	REGISTER_OBJECT_CLASS(AudioSource, SpatialNode)

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
	void SetAudioBuffer(AudioBuffer* audioBuffer);
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
	void AttachBuffer(AudioBuffer* clip);
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
	WeakPtr<Audio> _audio;
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
	SharedPtr<AudioBuffer> _audioBuffer;
};

}