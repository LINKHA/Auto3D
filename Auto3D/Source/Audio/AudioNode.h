#pragma once
#include "Scene/SpatialNode.h"

namespace Auto3D
{
/// Audio fUsage 
namespace AudioUsage
{
	enum Type
	{
		STATIC = 0,
		DYNAMIC,
	};
};

class Audio;

class AUTO_API AudioNode : public SpatialNode
{
	REGISTER_OBJECT_CLASS(AudioNode, SpatialNode)

public:
	/// Constructor
	AudioNode();
	/// Destructor
	~AudioNode();
	/// Register factory and attributes.
	static void RegisterObject();
	/// Set audio Usage
	void SetUsage(AudioUsage::Type fre) { _fre = fre; }
	/// Get audio Usage
	AudioUsage::Type GetUsage() { return _fre; }
protected:
	/// Audio weak ptr
	WeakPtr<Audio> _audio;
	/// Audio Usage 
	AudioUsage::Type _fre;
};

}