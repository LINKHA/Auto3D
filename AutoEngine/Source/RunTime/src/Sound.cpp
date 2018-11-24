#include "Sound.h"
#include "Ambient.h"

namespace Auto3D {

Sound::Sound(Ambient* ambient)
	: Super(ambient)
{
}


Sound::~Sound()
{
}

void Sound::RegisterObject(Ambient* ambient)
{
	ambient->RegisterFactory<Sound>(RESOURCE_ATTACH);
}

}