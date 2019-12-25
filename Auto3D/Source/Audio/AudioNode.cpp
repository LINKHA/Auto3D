#include "AudioNode.h"
#include "Audio.h"

#include "Debug/DebugNew.h"
#include "Core/Modules/ModuleManager.h"
#include "AudioListener.h"

namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AAudioNode)
	.constructor<>()
	;

	REGISTER_CALSS_FACTORY_IMP(AAudioListener)
	.constructor<>()
	.property("vel", &AAudioListener::GetVel, &AAudioListener::SetVel)
	(
	metadata(SERIALIZABLE, "")
	)
	.property("oriAt", &AAudioListener::GetOriAt, &AAudioListener::SetOriAt)
	(
		metadata(SERIALIZABLE, "")
		)
	.property("oriUp", &AAudioListener::GetOriUp, &AAudioListener::SetOriUp)
	(
		metadata(SERIALIZABLE, "")
		)
	;
}

AAudioNode::AAudioNode() :
	_fre(EAudioUsage::STATIC)
{
	_audio = GModuleManager::Get().AudioModule();
	// If you haven't created AAudio, create it
	if (!_audio)
	{
		GModuleManager::Get().RecreateAudioModule();
		_audio = GModuleManager::Get().AudioModule();
	}
}
AAudioNode::~AAudioNode()
{

}


}