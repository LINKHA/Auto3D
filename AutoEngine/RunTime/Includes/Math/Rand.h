#ifndef RAND_H_
#define RAND_H_
#include <basetsd.h>
#include "AUMathBase.h"

MATH_BEGIN
class Rand
{
public:

	Rand(UINT32 seed = 0)
	{
		SetSeed(seed);
	}

	UINT32 Get()
	{
		UINT32 t;
		t = x ^ (x << 11);
		x = y; y = z; z = w;
		return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	}

	inline static float GetFloatFromInt(UINT32 value)
	{
		// take 23 bits of integer, and divide by 2^23-1
		return float(value & 0x007FFFFF) * (1.0f / 8388607.0f);
	}

	inline static UINT8 GetByteFromInt(UINT32 value)
	{
		// take the most significant byte from the 23-bit value
		return UINT32(value >> (23 - 8));
	}

	// random number between 0.0 and 1.0
	float GetFloat()
	{
		return GetFloatFromInt(Get());
	}

	// random number between -1.0 and 1.0
	float GetSignedFloat()
	{
		return GetFloat() * 2.0f - 1.0f;
	}

	void SetSeed(UINT32 seed)
	{
		x = seed;
		y = x * 1812433253U + 1;
		z = y * 1812433253U + 1;
		w = z * 1812433253U + 1;
	}

	UINT32 GetSeed() const { return x; }

private:
	UINT32 x, y, z, w;

};
MATH_END



#endif
