#include "Audio.h"
#include "AutoOAL.h"
#include "DebugNew.h"


namespace Auto3D {

Audio::Audio(Ambient* ambient)
	:Super(ambient)
{
}


Audio::~Audio()
{

	ALFWShutdownOpenAL();

	ALFWShutdown();
}


void Audio::Init()
{
	ALFWInit();
	if (!ALFWInitOpenAL())
	{
		ErrorString("Failed to initialize OpenAL");
		ALFWShutdown();
	}

}


}