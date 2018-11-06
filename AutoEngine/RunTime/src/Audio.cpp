#include "Audio.h"
#include "AutoOAL.h"


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
	// Initialize Framework
	ALFWInit();

	//ALFWprintf("PlayStatic Test Application\n");

	if (!ALFWInitOpenAL())
	{
		ErrorString("Failed to initialize OpenAL");
		ALFWShutdown();
	}

}


}