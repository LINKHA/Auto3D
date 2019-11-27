#include "AudioNode.h"
#include "Audio.h"

#include "../Debug/DebugNew.h"
#include "../Core/Modules/ModuleManager.h"

namespace Auto3D
{

AudioNode::AudioNode() :
	_fre(AudioUsage::STATIC)
{
	_audio = ModuleManager::Get().AudioModule();
	// If you haven't created Audio, create it
	if (!_audio)
	{
		ModuleManager::Get().RecreateAudioModule();
		_audio = ModuleManager::Get().AudioModule();
	}
}
AudioNode::~AudioNode()
{

}

void AudioNode::RegisterObject()
{
	RegisterFactory<AudioNode>();
}

	



}