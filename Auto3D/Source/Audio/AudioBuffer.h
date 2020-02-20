#pragma once
#include "AutoConfig.h"

namespace Auto3D
{
namespace EAudioSourceUsage
{
	enum Type
	{
		DEFAULT = 0,
		IMMUTABLE,
		DYNAMIC,
	};
};

class GAudioModule;
class OSound;
/// AAudio buffer
class AUTO_API FAudioBuffer
{
public:
	/// Constructor
	FAudioBuffer();
	/// Destructor
	~FAudioBuffer();
	/// Create the GPU-side vertex buffer. Return true on success.
	bool Create(OSound* sound);
	/// Get buffer.
	unsigned& Buffer() { return _buffer; }
	/// Get source.
	unsigned& Source() { return _source; }
private:
	/// OpenGL buffer object identifier.
	unsigned _buffer;
	/// Audio source.
	unsigned _source;
	/// Audio source usage.
	EAudioSourceUsage::Type _usage;
};

}
