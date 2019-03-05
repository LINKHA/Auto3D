#pragma once
#include "../Object/GameManager.h"

/** Opaque device handle */
typedef struct ALCdevice_struct ALCdevice;
/** Opaque context handle */
typedef struct ALCcontext_struct ALCcontext;



namespace Auto3D 
{

class AudioListener;

class AudioSource;

/// Audio sub system ,Process all sound sources and calculate sound
class AUTO_API Audio : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(Audio, BaseSubsystem)
public:
	/// Construct
	Audio();

	~Audio();
	
	void SetListener(AudioListener* listener);

	void SetListenerValue(Vector3F position, Vector3F listenerVel, Vector3F listenerOriAt, Vector3F listenerOriUp);
	/// The first person delays ms according to the buffer play
	void SourcePlay(unsigned source,int delay = 0);
	/// The first person delays ms according to the buffer pause
	void SourcePause(unsigned source, int delay);
	/// The first person delays ms according to the buffer stop
	void SourceStop(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer rewind
	void SourceRewind(unsigned source, int delay = 0);
	/// Update all dynamic listener and source
	void UpdateAudioData();
private:
	/// OpenAL device
	ALCdevice* _device;
	/// OpenAL context
	ALCcontext* _context;

	SharedPtr<AudioListener> _listener;

	HashMap<unsigned, AudioSource*> _source;
};
/// Register Audio related object factories and attributes.
AUTO_API void RegisterAudioLibrary();

}