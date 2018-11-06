#include "AudioClip.h"

namespace Auto3D {

AudioClip::AudioClip(Ambient* ambient, __String path)
	: Super(ambient)
	, _path(path)
{
}

AudioClip::~AudioClip()
{
}

}