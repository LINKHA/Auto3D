#include "Audio.h"
#include "AutoOAL.h"
#include "NewDef.h"


namespace Auto3D {




Audio::Audio(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
	/* initialize OpenAL context, asking for 48.0kHz to match HRIR data */
	//ALCdevice *device;
	//ALCcontext *ctx;
	//const ALCint contextAttr[] = { ALC_FREQUENCY,48000,0 };

	//device = alcOpenDevice(NULL);
	//ctx = alcCreateContext(device, contextAttr);
	//alcMakeContextCurrent(ctx);

}


Audio::~Audio()
{	
	//ALCdevice *device;
	//ALCcontext *ctx;
	//ctx = alcGetCurrentContext();
	//if (ctx == NULL)
	//	return;

	//device = alcGetContextsDevice(ctx);

	//alcMakeContextCurrent(NULL);
	//alcDestroyContext(ctx);
	//alcCloseDevice(device);
}

}