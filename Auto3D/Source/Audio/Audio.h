#pragma once
#include "Core/GameManager.h"

/// Opaque device handle 
using ALCdevice = struct ALCdevice_struct;
/// Opaque context handle 
using ALCcontext = struct ALCcontext_struct;


namespace Auto3D 
{

namespace EAudioSourceState
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


class AAudioListener;

class AAudioSource;

/// AAudio sub system ,Process all sound sources and calculate sound
class AUTO_API AAudio : public ABaseModule
{
	REGISTER_OBJECT_CLASS(AAudio, ABaseModule)
public:
	/// Construct
	AAudio();
	/// Destructor
	~AAudio();
	/// Add source with source address and source
	void AddSource(unsigned sourceID, AAudioSource* source);
	/// Set listener (if listener NULL this subsystem cant run)
	void SetListener(AAudioListener* listener);
	/// Set listener value
	void SetListenerValue(Vector3F position, Vector3F listenerVel, Vector3F listenerOriAt, Vector3F listenerOriUp);
	/// The first person delays ms according to the buffer play
	void SourcePlay(unsigned source,int delay = 0);
	/// The first person delays ms according to the buffer play
	void SourcePlay(AAudioSource* source, int delay = 0);
	/// The first person delays ms according to the buffer pause
	void SourcePause(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer pause
	void SourcePause(AAudioSource* source, int delay = 0);
	/// The first person delays ms according to the buffer stop
	void SourceStop(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer stop
	void SourceStop(AAudioSource* source, int delay = 0);
	/// The first person delays ms according to the buffer rewind
	void SourceRewind(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer rewind
	void SourceRewind(AAudioSource* source, int delay = 0);
	/// Set pitch of source address
	void SetPitch(unsigned source, float val);
	/// Set gain of source address
	void SetGain(unsigned source, float val);
	/// Set vel of source address
	void SetVel(unsigned source, Vector3F vel);
	/// Set get state of source
	EAudioSourceState::Type GetState(unsigned source);
	/// Set get state of source
	EAudioSourceState::Type GetState(AAudioSource* source);
	/// Get source of index
	const AAudioSource* GetSource(unsigned index);
	/// Return sources
	HashMap<unsigned, AAudioSource*>& Sources(AAudioSource* source) { return _sources; }
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
	SharedPtr<AAudioListener> _listener;
	/// Source array of hash
	HashMap<unsigned, AAudioSource*> _sources;
};
/// Register AAudio related object factories and attributes.
AUTO_API void RegisterAudioLibrary();

}