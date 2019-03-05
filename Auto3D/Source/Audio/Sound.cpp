#include "Sound.h"
#include "../IO/Stream.h"
#include "../Debug/Log.h"

#include "../Debug/DebugNew.h"

namespace Auto3D {

Sound::Sound()
{
}


Sound::~Sound()
{

}

void Sound::RegisterObject()
{
	RegisterFactory<Sound>();
}

bool Sound::BeginLoad(Stream& source)
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


bool Sound::LoadRaw(Stream& source)
{
	//unsigned dataSize = source.Size();
	//SetSize(dataSize);
	//return source.Read(_data.Get(), dataSize) == dataSize;
	return false;
}

}