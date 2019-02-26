#pragma once

#include "Math.h"

namespace Auto3D
{
//class Rand
//{
//public:
//
//	Rand(unsigned seed = 0)
//	{
//		SetSeed(seed);
//	}
//
//	static unsigned Get()
//	{
//		unsigned t;
//		t = x ^ (x << 11);
//		x = y; y = z; z = w;
//		return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
//	}
//
//	inline static float GetFloatFromInt(unsigned value)
//	{
//		// take 23 bits of integer, and divide by 2^23-1
//		return float(value & 0x007FFFFF) * (1.0f / 8388607.0f);
//	}
//
//	inline static unsigned char GetByteFromInt(unsigned value)
//	{
//		// take the most significant byte from the 23-bit value
//		return unsigned(value >> (23 - 8));
//	}
//
//	// random number between 0.0 and 1.0
//	static float GetFloat()
//	{
//		return GetFloatFromInt(Get());
//	}
//
//	// random number between -1.0 and 1.0
//	static float GetSignedFloat()
//	{
//		return GetFloat() * 2.0f - 1.0f;
//	}
//	// random number between 0 and RAND_MAX
//	static int GetInt(int t = RAND_MAX)
//	{
//		return t * GetFloat();
//	}
//	// random number between -RAND_MAX and RAND_MAX
//	static int GetSignedInt(int t = RAND_MAX)
//	{
//		return t * GetSignedFloat();
//	}
//	static void SetSeed(unsigned seed)
//	{
//		x = seed;
//		y = x * 1812433253U + 1;
//		z = y * 1812433253U + 1;
//		w = z * 1812433253U + 1;
//	}
//
//	static unsigned GetSeed() { return x; }
//
//private:
//	static unsigned x, y, z, w;
//
//};

/// Set the random seed. The default seed is 1.
AUTO_API void SetRandomSeed(unsigned seed);
/// Return the current random seed.
AUTO_API unsigned RandomSeed();
/// Return a random number between 0-32767. Should operate similarly to MSVC rand().
AUTO_API int Rand();
/// Return a standard normal distributed number.
AUTO_API float RandStandardNormal();

/// Return a random float between 0.0 (inclusive) and 1.0 (exclusive.)
inline float Random() { return Rand() / 32768.0f; }
/// Return a random float between 0.0 and range, inclusive from both ends.
inline float Random(float range) { return Rand() * range / 32767.0f; }
/// Return a random float between min and max, inclusive from both ends.
inline float Random(float min, float max) { return Rand() * (max - min) / 32767.0f + min; }
/// Return a random integer between 0 and range - 1.
inline int Random(int range) { return (Rand() * (range - 1) + 16384) / 32767; }
/// Return a random integer between min and max - 1.
inline int Random(int min, int max) { return (Rand() * (max - min - 1) + 16384) / 32767 + min; }
/// Return a random normal distributed number with the given mean value and variance.
inline float RandomNormal(float meanValue, float variance) { return RandStandardNormal() * sqrtf(variance) + meanValue; }

}
