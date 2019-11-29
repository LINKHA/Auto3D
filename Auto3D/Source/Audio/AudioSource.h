#pragma once
#include "AudioNode.h"
#include "Audio.h"

namespace Auto3D 
{

class ASound;
class AAudioBuffer;
/// Responsible for the operation of 3d sound source
class AUTO_API AAudioSource : public AAudioNode
{
	REGISTER_OBJECT_CLASS(AAudioSource, AAudioNode)
public:
	/// The constructor
	AAudioSource();
	/// The destructor
	~AAudioSource();
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
	void SetSound(ASound* sound);
	/// Get audio buffer
	AAudioBuffer* GetBuffer() { return  _buffer; }
	/// Get source state
	EAudioSourceState::Type GetState();
private:
	/// AAudio sound resource
	SharedPtr<ASound> _sound;
	/// AAudio buffer
	SharedPtr<AAudioBuffer> _buffer;
	/// Pitch
	float _pitch;
	/// Gain
	float _gain;
	/// Vel
	Vector3F _vel;
};

}