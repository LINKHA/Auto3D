#include "AudioNode.h"
#include "Audio.h"

#include "Debug/DebugNew.h"
#include "Core/Modules/ModuleManager.h"

namespace Auto3D
{

REGISTER_CLASS
{
	using namespace rttr;
	registration::class_<AAudioNode>("AAudioNode")
	.constructor<>()
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

void AAudioNode::RegisterObject()
{
	RegisterFactory<AAudioNode>();
}

	



}