#pragma once
#include "../Scene/SpatialNode.h"

namespace Auto3D
{
/// Audio fUsage 
struct __AudioUsage
{
	enum _AudioUsage
	{
		Static = 0,
		Dynamic,
	};
};
using AudioUsage = __AudioUsage::_AudioUsage;

class Audio;

class AUTO_API AudioNode : public SpatialNode
{
	REGISTER_OBJECT_CLASS(AudioNode, SpatialNode)

public:
	AudioNode();
	~AudioNode();
	/// Register factory and attributes.
	static void RegisterObject();
	/// Set audio Usage
	void SetUsage(AudioUsage fre) { _fre = fre; }
	/// Get audio Usage
	AudioUsage GetUsage() { return _fre; }
protected:
	/// Audio weak ptr
	WeakPtr<Audio> _audio;
	/// Audio Usage 
	AudioUsage _fre;
};

}