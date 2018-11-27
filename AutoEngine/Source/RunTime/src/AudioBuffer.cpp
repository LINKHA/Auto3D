#include "AudioBuffer.h"
#include "DebugNew.h"

namespace Auto3D {

AudioBuffer::AudioBuffer(Ambient* ambient, STRING path)
	: Super(ambient)
	, _data(path)
{
	Print(GetData());
}

AudioBuffer::~AudioBuffer()
{
}

}