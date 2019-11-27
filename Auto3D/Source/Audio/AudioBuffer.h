#pragma once
#include "Container/Ptr.h"

namespace Auto3D
{
namespace AudioSourceUsage
{
	enum Type
	{
		DEFAULT = 0,
		IMMUTABLE,
		DYNAMIC,
	};
};

class Audio;
class Sound;
/// Audio buffer
class AUTO_API AudioBuffer : public RefCounted
{
public:
	AudioBuffer();
	
	~AudioBuffer();
	/// Create the GPU-side vertex buffer. Return true on success.
	bool Create(Sound* sound);

	unsigned& Buffer() { return _buffer; }

	unsigned& Source() { return _source; }
private:
	WeakPtr<Audio> _audio;
	
	/// OpenGL buffer object identifier.
	unsigned _buffer;

	unsigned _source;

	AudioSourceUsage::Type _usage;
};

}
