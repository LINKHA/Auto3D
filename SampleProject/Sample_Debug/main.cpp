/* footsteps.c
 *
 * To compile:
 *   gcc -o footsteps footsteps.c -lopenal
 *
 * Requires data "footsteps.raw", which is any signed-16bit
 * mono audio data (no header!); assumed samplerate is 44.1kHz.
 *
 */

#include <stdlib.h>
#include <stdio.h>
//#include <unistd.h>  /* for usleep() */
#include <math.h>    /* for sqrtf() */
#include <time.h>    /* for time(), to seed srand() */

 /* OpenAL headers */
#include <OpenAL/include/AL/al.h>
#include <OpenAL/include/AL/alc.h>
#include <OpenAL/include/AL/alext.h>

/* load a file into memory, returning the buffer and
 * setting bufsize to the size-in-bytes */
void* load(char *fname, long *bufsize) {
	FILE* fp = fopen(fname, "rb");
	fseek(fp, 0L, SEEK_END);
	long len = ftell(fp);
	rewind(fp);
	void *buf = malloc(len);
	fread(buf, 1, len, fp);
	fclose(fp);
	*bufsize = len;
	return buf;
}

/* randomly displace 'a' by one meter +/- in x or z */
void randWalk(float *a) {
	int r = rand() & 0x3;
	switch (r) {
	case 0: a[0] -= 1.; break;
	case 1: a[0] += 1.; break;
	case 2: a[2] -= 1.; break;
	case 3: a[2] += 1.; break;
	}
	printf("Walking to: %.1f,%.1f,%.1f\n", a[0], a[1], a[2]);
}

int main(int argc, char *argv[]) {
	/* current position and where to walk to... start just 1m ahead */
	float curr[3] = { 0.,0.,-1. };
	float targ[3] = { 0.,0.,-1. };

	/* initialize OpenAL context, asking for 44.1kHz to match HRIR data */
	ALCint contextAttr[] = { ALC_FREQUENCY,44100,0 };
	ALCdevice* device = alcOpenDevice(NULL);
	ALCcontext* context = alcCreateContext(device, contextAttr);
	alcMakeContextCurrent(context);

	/* listener at origin, facing down -z (ears at 1.5m height) */
	alListener3f(AL_POSITION, 0., 1.5, 0.);
	alListener3f(AL_VELOCITY, 0., 0., 0.);
	float orient[6] = { /*fwd:*/ 0., 0., -1., /*up:*/ 0., 1., 0. };
	alListenerfv(AL_ORIENTATION, orient);

	/* this will be the source of ghostly footsteps... */
	ALuint source;
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1.);
	alSourcef(source, AL_GAIN, 1.);
	alSource3f(source, AL_POSITION, curr[0], curr[1], curr[2]);
	alSource3f(source, AL_VELOCITY, 0., 0., 0.);
	alSourcei(source, AL_LOOPING, AL_TRUE);

	/* allocate an OpenAL buffer and fill it with monaural sample data */
	ALuint buffer;
	alGenBuffers(1, &buffer);
	{
		long dataSize;
		const ALvoid* data = load("SoundTest.wav", &dataSize);
		/* for simplicity, assume raw file is signed-16b at 44.1kHz */
		alBufferData(buffer, AL_FORMAT_MONO16, data, dataSize, 44100);
		free((void*)data);
	}
	alSourcei(source, AL_BUFFER, buffer);

	/* state initializations for the upcoming loop */
	//srand((int)time(NULL));
	float dt = 1. / 60.;
	float vel = 0.8 * dt;
	float closeEnough = 0.05;

	/** BEGIN! **/
	alSourcePlay(source);

	fflush(stderr); /* in case OpenAL reported an error earlier */

	/* loop forever... walking to random, adjacent, integer coordinates */
	for (;;) {
		float dx = targ[0] - curr[0];
		float dy = targ[1] - curr[1];
		float dz = targ[2] - curr[2];
		float dist = sqrtf(dx*dx + dy * dy + dz * dz);
		if (dist < closeEnough) randWalk(targ);
		else {
			float toVel = vel / dist;
			float v[3] = { dx*toVel, dy*toVel, dz*toVel };
			curr[0] += v[0];
			curr[1] += v[1];
			curr[2] += v[2];

			alSource3f(source, AL_POSITION, curr[0], curr[1], curr[2]);
			alSource3f(source, AL_VELOCITY, v[0], v[1], v[2]);
			Sleep((int)(1e6*dt));
		}
	}

	/* cleanup that should be done when you have a proper exit... ;) */
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	alcDestroyContext(context);
	alcCloseDevice(device);

	return 0;
}