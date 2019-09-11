#include "AudioNode.h"
#include "Audio.h"

#include "../Debug/DebugNew.h"
#include "../Engine/ModuleManager.h"

namespace Auto3D
{

AudioNode::AudioNode() :
	_fre(AudioUsage::STATIC)
{
	_audio = Module<Audio>();
	// If you haven't created Audio, create it
	if (!_audio)
	{
		RegisterModule(_audio = new Audio());
		ModuleManager::Get()._audio = _audio;
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