#include "Audio.h"
#include "AutoOAL.h"
#include "NewDef.h"


namespace Auto3D {

Audio::Audio(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}


Audio::~Audio()
{
	alcDestroyContext(context);
	alcCloseDevice(device);
}


void Audio::Init()
{
	/* initialize OpenAL context, asking for 48.0kHz to match HRIR data */
	ALCint contextAttr[] = { ALC_FREQUENCY,48000,0 };
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, contextAttr);
	alcMakeContextCurrent(context);
}


}