#pragma once
#include "Container/Ptr.h"

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

class AAudio;
class ASound;
/// AAudio buffer
class AUTO_API AAudioBuffer : public FRefCounted
{
public:
	/// Constructor
	AAudioBuffer();
	/// Destructor
	~AAudioBuffer();
	/// Create the GPU-side vertex buffer. Return true on success.
	bool Create(ASound* sound);
	/// Get buffer.
	unsigned& Buffer() { return _buffer; }
	/// Get source.
	unsigned& Source() { return _source; }
private:
	/// Get audio moudle for weak.
	TWeakPtr<AAudio> _audio;
	/// OpenGL buffer object identifier.
	unsigned _buffer;
	/// Audio source.
	unsigned _source;
	/// Audio source usage.
	EAudioSourceUsage::Type _usage;
};

}
