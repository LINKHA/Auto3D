#include "AudioRun.h"
#include "Framework.h"

void Run()
{

	ALuint      uiBuffer;
	ALuint      uiSource;
	ALint       iState;

	// Initialize Framework
	ALFWInit();

	ALFWprintf("PlayStatic Test Application\n");

	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
	}

	// Generate an AL Buffer
	alGenBuffers(1, &uiBuffer);

	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath("../Resource/sound/SoundTest.wav"), uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", ALFWaddMediaPath("../Resource/sound/SoundTest.wav"));
	}

	// Generate a Source to playback the Buffer
	alGenSources(1, &uiSource);

	// Attach Source to Buffer
	alSourcei(uiSource, AL_BUFFER, uiBuffer);

	// Play Source
	alSourcePlay(uiSource);
	ALFWprintf("Playing Source ");

	do
	{
		Sleep(100);
		ALFWprintf(".");
		// Get Source State
		alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
	} while (iState == AL_PLAYING);

	ALFWprintf("\n");

	// Clean up by deleting Source(s) and Buffer(s)
	alSourceStop(uiSource);
	alDeleteSources(1, &uiSource);
	alDeleteBuffers(1, &uiBuffer);

	ALFWShutdownOpenAL();

	ALFWShutdown();
}