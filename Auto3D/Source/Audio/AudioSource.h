#pragma once
#include "AudioNode.h"
#include "Audio.h"

namespace Auto3D 
{

class Sound;
class AudioBuffer;




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
	* @brief : Attach buffer for point
	*/
	void SetSound(Sound* sound);

	AudioBuffer* GetBuffer() { return  _buffer; }

	AudioSourceState GetState();
private:

	/// audio buffer
	SharedPtr<Sound> _sound;

	SharedPtr<AudioBuffer> _buffer;

	float _pitch;

	float _gain;

	Vector3F _vel;
};

}