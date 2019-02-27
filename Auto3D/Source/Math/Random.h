#pragma once

#include "Math.h"

namespace Auto3D
{

AUTO_API void SetRandomSeed(unsigned seed = 0);
AUTO_API unsigned Rand();
AUTO_API unsigned RandomSeed();
AUTO_API float GetFloatFromInt(unsigned value);
AUTO_API unsigned char GetByteFromInt(unsigned value);
// random number between 0.0 and 1.0
AUTO_API float RandomFloat();
// random number between -1.0 and 1.0
AUTO_API float RandomSignedFloat();
// random number between 0 and RAND_MAX
AUTO_API int RandomInt(int t = RAND_MAX);
// random number between -RAND_MAX and RAND_MAX
AUTO_API int RandomSignedInt(int t = RAND_MAX);
/// Return a random float between 0.0 (inclusive) and 1.0 (exclusive.)
inline float Random() { return RandomFloat(); }
/// Return a random float between 0 and range, inclusive from both en
inline float Random(float range) { return RandomFloat() * range; }
// random number between 0 and max
inline int Random(int max) {return RandomInt(max);}
// random number between min and max
inline int Random(int min, int max) { return RandomInt( max - min) + min; }

}
