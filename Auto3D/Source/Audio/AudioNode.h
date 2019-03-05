#pragma once
#include "../Scene/SpatialNode.h"

namespace Auto3D
{
/// Audio fFrequency 
struct __AudioFrequency
{
	enum _AudioFrequency
	{
		Static = 0,
		Dynamic,
	};
};
using AudioFrequency = __AudioFrequency::_AudioFrequency;

class Audio;

class AUTO_API AudioNode : public SpatialNode
{
	REGISTER_OBJECT_CLASS(AudioNode, SpatialNode)

public:
	AudioNode();
	~AudioNode();
	/// Register factory and attributes.
	static void RegisterObject();
	/// Set audio frequency
	void SetFrequency(AudioFrequency fre) { _fre = fre; }
	/// Get audio frequency
	AudioFrequency GetFrequency() { return _fre; }
protected:
	/// Audio weak ptr
	WeakPtr<Audio> _audio;
	/// Audio frequency 
	AudioFrequency _fre;
};

}