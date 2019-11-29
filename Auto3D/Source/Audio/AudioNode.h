#pragma once
#include "Scene/SpatialNode.h"

namespace Auto3D
{
/// AAudio fUsage 
namespace EAudioUsage
{
	enum Type
	{
		STATIC = 0,
		DYNAMIC,
	};
};

class AAudio;

class AUTO_API AAudioNode : public ASpatialNode
{
	REGISTER_OBJECT_CLASS(AAudioNode, ASpatialNode)

public:
	/// Constructor
	AAudioNode();
	/// Destructor
	~AAudioNode();
	/// Register factory and attributes.
	static void RegisterObject();
	/// Set audio Usage
	void SetUsage(EAudioUsage::Type fre) { _fre = fre; }
	/// Get audio Usage
	EAudioUsage::Type GetUsage() { return _fre; }
protected:
	/// AAudio weak ptr
	TWeakPtr<AAudio> _audio;
	/// AAudio Usage 
	EAudioUsage::Type _fre;
};

}