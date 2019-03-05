#include "AudioBuffer.h"
#include "../IO/Stream.h"
#include "../Debug/Log.h"

#include "../Debug/DebugNew.h"

namespace Auto3D {

AudioBuffer::AudioBuffer()
{
}


AudioBuffer::~AudioBuffer()
{

}

void AudioBuffer::RegisterObject()
{
	RegisterFactory<AudioBuffer>();
}

bool AudioBuffer::BeginLoad(Stream& source)
{
	source.Seek(0);
	void *buf = malloc(source.Size());
	if (!source.Read(buf, source.Size()))
	{
		ErrorString("Could not load audio buffer " + source.Name());
		return false;
	}

	SetData(buf);
	SetSize(source.Size());

	return true;
}

}