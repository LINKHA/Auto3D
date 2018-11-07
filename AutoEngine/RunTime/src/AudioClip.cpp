#include "AudioClip.h"

namespace Auto3D {

AudioClip::AudioClip(Ambient* ambient, STRING path)
	: Super(ambient)
	, _path(path)
{
}

AudioClip::~AudioClip()
{
}

}