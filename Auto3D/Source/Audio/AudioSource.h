#pragma once
#include "AudioNode.h"
#include "Audio.h"

namespace Auto3D 
{

class Sound;
class AudioBuffer;
/// Responsible for the operation of 3d sound source
class AUTO_API AudioSource : public AudioNode
{
	REGISTER_OBJECT_CLASS(AudioSource, AudioNode)
public:
	/// The constructor
	AudioSource();
	/// The destructor
	~AudioSource();
	/// Register object factory.
	static void RegisterObject();
	/// Plays the active audioclip at (future) scheduled time. If time < 0 it specifies a delay
	void Play(int delayTime = 0);
	/// Pauses the active audioclip
	void Pause(int delayTime = 0);
	/// Stops the active audio clip
	void Stop(int delayTime = 0);
	/// Rewind audio clip
	void Rewind(int delayTime = 0);
	/// Attach buffer for point
	void SetSound(Sound* sound);
	/// Get audio buffer
	AudioBuffer* GetBuffer() { return  _buffer; }
	/// Get source state
	AudioSourceState::Type GetState();
private:
	/// Audio sound resource
	SharedPtr<Sound> _sound;
	/// Audio buffer
	SharedPtr<AudioBuffer> _buffer;
	/// Pitch
	float _pitch;
	/// Gain
	float _gain;
	/// Vel
	Vector3F _vel;
};

}