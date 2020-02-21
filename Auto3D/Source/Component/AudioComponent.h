#pragma once
#include "Component/ActorComponent.h"

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

class AUTO_API AAudioComponent : public AActorComponent
{
	DECLARE_A_CLASS(AAudioComponent, AActorComponent)
public:
	/// Constructor
	AAudioComponent();
	/// Destructor
	~AAudioComponent();
	/// Set audio Usage
	void SetUsage(EAudioUsage::Type fre) { _fre = fre; }
	/// Get audio Usage
	EAudioUsage::Type GetUsage() { return _fre; }
protected:
	/// AAudio Usage 
	EAudioUsage::Type _fre;
};

}