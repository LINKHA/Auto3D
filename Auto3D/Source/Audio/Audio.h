#pragma once
#include "../Object/GameManager.h"

/** Opaque device handle */
typedef struct ALCdevice_struct ALCdevice;
/** Opaque context handle */
typedef struct ALCcontext_struct ALCcontext;

namespace Auto3D {

/// Audio sub system ,Process all sound sources and calculate sound
class AUTO_API Audio : public Subsystem
{
	REGISTER_OBJECT_CLASS(Audio, Subsystem)
public:
	/// Construct
	Audio();

	~Audio();
private:
	/// OpenAL device
	ALCdevice* _device;
	/// OpenAL context
	ALCcontext* _context;
};


}