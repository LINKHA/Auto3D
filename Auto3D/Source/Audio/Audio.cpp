#include "Audio.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace Auto3D {

const ALCint contextAttr[] = { ALC_FREQUENCY,48000,0 };

Audio::Audio()
{
	_device = alcOpenDevice(NULL);
	_context = alcCreateContext(_device, contextAttr);
	alcMakeContextCurrent(_context);
}


Audio::~Audio()
{
	alcDestroyContext(_context);
	alcCloseDevice(_device);
}


}