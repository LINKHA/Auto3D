#include "Random.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

static unsigned x = 0;
static unsigned y = x * 1812433253U + 1;
static unsigned z = y * 1812433253U + 1;
static unsigned w = z * 1812433253U + 1;

void SetRandomSeed(unsigned seed)
{
	x = seed;
	y = x * 1812433253U + 1;
	z = y * 1812433253U + 1;
	w = z * 1812433253U + 1;
}

unsigned Rand()
{
	unsigned t;
	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

unsigned RandomSeed() { return x; }

float GetFloatFromInt(unsigned value)
{
	// take 23 bits of integer, and divide by 2^23-1
	return float(value & 0x007FFFFF) * (1.0f / 8388607.0f);
}

unsigned char GetByteFromInt(unsigned value)
{
	// take the most significant byte from the 23-bit value
	return unsigned(value >> (23 - 8));
}

// random number between 0.0 and 1.0
float RandomFloat()
{
	return GetFloatFromInt(Rand());
}

// random number between -1.0 and 1.0
float RandomSignedFloat()
{
	return RandomFloat() * 2.0f - 1.0f;
}
// random number between 0 and RAND_MAX
int RandomInt(int t)
{
	return (t + 1) * RandomFloat();
}

// random number between -RAND_MAX and RAND_MAX
int RandomSignedInt(int t)
{
	return (t + 1) * RandomSignedFloat();
}

}
