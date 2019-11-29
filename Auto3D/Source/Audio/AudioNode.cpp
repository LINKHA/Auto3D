#include "AudioNode.h"
#include "Audio.h"

#include "Debug/DebugNew.h"
#include "Core/Modules/ModuleManager.h"

namespace Auto3D
{

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