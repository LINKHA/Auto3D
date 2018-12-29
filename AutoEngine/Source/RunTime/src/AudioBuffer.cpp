#include "AudioBuffer.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "MemoryDef.h"

#include "NewDef.h"

namespace Auto3D {

AudioBuffer::AudioBuffer(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}


AudioBuffer::~AudioBuffer()
{
	SafeFree(_data);
}

void AudioBuffer::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<AudioBuffer>();
}

bool AudioBuffer::BeginLoad(Deserializer& source)
{
	source.Seek(0);
	void *buf = malloc(source.GetSize());
	if (!source.Read(buf, source.GetSize()))
	{
		AutoCout << "Could not load audio buffer " + source.GetName() << AutoEndl;
		return false;
	}

	SetData(buf);
	SetSize(source.GetSize());

	return true;
}

}