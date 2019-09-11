#pragma once
#include "../Base/GameManager.h"

/// Opaque device handle 
using ALCdevice = struct ALCdevice_struct;
/// Opaque context handle 
using ALCcontext = struct ALCcontext_struct;


namespace Auto3D 
{

namespace AudioSourceState
{
	enum Type
	{
		DEFAULT,
		INITIAL,
		PLAYING,
		PAUSED,
		STOPPED
	};
};


class AudioListener;

class AudioSource;

/// Audio sub system ,Process all sound sources and calculate sound
class AUTO_API Audio : public BaseModule
{
	REGISTER_OBJECT_CLASS(Audio, BaseModule)
public:
	/// Construct
	Audio();
	/// Destructor
	~Audio();
	/// Add source with source address and source
	void AddSource(unsigned sourceID, AudioSource* source);
	/// Set listener (if listener NULL this subsystem cant run)
	void SetListener(AudioListener* listener);
	/// Set listener value
	void SetListenerValue(Vector3F position, Vector3F listenerVel, Vector3F listenerOriAt, Vector3F listenerOriUp);
	/// The first person delays ms according to the buffer play
	void SourcePlay(unsigned source,int delay = 0);
	/// The first person delays ms according to the buffer play
	void SourcePlay(AudioSource* source, int delay = 0);
	/// The first person delays ms according to the buffer pause
	void SourcePause(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer pause
	void SourcePause(AudioSource* source, int delay = 0);
	/// The first person delays ms according to the buffer stop
	void SourceStop(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer stop
	void SourceStop(AudioSource* source, int delay = 0);
	/// The first person delays ms according to the buffer rewind
	void SourceRewind(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer rewind
	void SourceRewind(AudioSource* source, int delay = 0);
	/// Set pitch of source address
	void SetPitch(unsigned source, float val);
	/// Set gain of source address
	void SetGain(unsigned source, float val);
	/// Set vel of source address
	void SetVel(unsigned source, Vector3F vel);
	/// Set get state of source
	AudioSourceState::Type GetState(unsigned source);
	/// Set get state of source
	AudioSourceState::Type GetState(AudioSource* source);
	/// Get source of index
	const AudioSource* GetSource(unsigned index);
	/// Return sources
	HashMap<unsigned, AudioSource*>& Sources(AudioSource* source) { return _sources; }
	/// Update all dynamic listener and source
	void Update();
	/// Return is initialized flag
	bool IsInitialized();
private:
	/// The first person delays ms according to the buffer play
	void CallSourcePlay(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer pause
	void CallSourcePause(unsigned source, int delay);
	/// The first person delays ms according to the buffer stop
	void CallSourceStop(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer rewind
	void CallSourceRewind(unsigned source, int delay = 0);
	/// OpenAL device
	ALCdevice* _device;
	/// OpenAL context
	ALCcontext* _context;
	/// Listener
	SharedPtr<AudioListener> _listener;
	/// Source array of hash
	HashMap<unsigned, AudioSource*> _sources;
};
/// Register Audio related object factories and attributes.
AUTO_API void RegisterAudioLibrary();

}