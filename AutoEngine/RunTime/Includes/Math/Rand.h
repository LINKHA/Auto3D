#pragma once
#include "MathBase.h"
AUTO_BEGIN
class Rand
{
public:

	Rand(UInt32 seed = 0)
	{
		SetSeed(seed);
	}

	UInt32 Get()
	{
		UInt32 t;
		t = x ^ (x << 11);
		x = y; y = z; z = w;
		return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	}

	inline static float GetFloatFromInt(UInt32 value)
	{
		// take 23 bits of integer, and divide by 2^23-1
		return float(value & 0x007FFFFF) * (1.0f / 8388607.0f);
	}

	inline static UInt8 GetByteFromInt(UInt32 value)
	{
		// take the most significant byte from the 23-bit value
		return UInt32(value >> (23 - 8));
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
	// random number between 0 and RAND_MAX
	int GetInt(int t = RAND_MAX)
	{
		return t * GetFloat();
	}
	// random number between -RAND_MAX and RAND_MAX
	int GetSignedInt(int t = RAND_MAX)
	{
		return t * GetSignedFloat();
	}
	void SetSeed(UInt32 seed)
	{
		x = seed;
		y = x * 1812433253U + 1;
		z = y * 1812433253U + 1;
		w = z * 1812433253U + 1;
	}

	UInt32 GetSeed() const { return x; }

private:
	UInt32 x, y, z, w;

};
AUTO_END

