#pragma once

#include "AutoMath.h"

namespace Auto3D
{
/// Set the seeds 
AUTO_API void SetRandomSeed(unsigned seed = 0);
/// Randomize Numbers by seed
AUTO_API unsigned Rand();
/// Return seed
AUTO_API unsigned RandomSeed();
/// Calculates floating point Numbers based on integers
AUTO_API float GetFloatFromInt(unsigned value);
/// Calculates bytes based on integer
AUTO_API unsigned char GetByteFromInt(unsigned value);

/// Random number between 0.0 and 1.0
AUTO_API float RandomFloat();
/// Random number between -1.0 and 1.0
AUTO_API float RandomSignedFloat();
/// Random number between 0 and RAND_MAX
AUTO_API int RandomInt(int t = RAND_MAX);
/// Random number between -RAND_MAX and RAND_MAX
AUTO_API int RandomSignedInt(int t = RAND_MAX);
/// Return a random float between 0.0 (inclusive) and 1.0 (exclusive.)
inline float Random() { return RandomFloat(); }
/// Return a random float between 0 and range, inclusive from both en
inline float Random(float range) { return RandomFloat() * range; }
/// Random number between 0 and max
inline int Random(int max) {return RandomInt(max);}
/// Random number between min and max
inline int Random(int min, int max) { return RandomInt( max - min) + min; }

}
