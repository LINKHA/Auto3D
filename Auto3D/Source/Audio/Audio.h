#pragma once
#include "Container/HashMap.h"
#include "Math/Vector3.h"
#include "Container/Singleton.h"

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


class AAudioListenerComponent;

class AAudioSourceComponent;

/// AAudio sub system ,Process all sound sources and calculate sound
class AUTO_API GAudioModule
{
	REGISTER_SINGLETON(GAudioModule)
public:
	/// Construct
	GAudioModule();
	/// Destructor
	~GAudioModule();
	/// Add source with source address and source
	void AddSource(unsigned sourceID, AAudioSourceComponent* source);
	/// Set listener (if listener NULL this subsystem cant run)
	void SetListener(AAudioListenerComponent* listener);
	/// Set listener value
	void SetListenerValue(TVector3F position, TVector3F listenerVel, TVector3F listenerOriAt, TVector3F listenerOriUp);
	/// The first person delays ms according to the buffer play
	void SourcePlay(unsigned source,int delay = 0);
	/// The first person delays ms according to the buffer play
	void SourcePlay(AAudioSourceComponent* source, int delay = 0);
	/// The first person delays ms according to the buffer pause
	void SourcePause(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer pause
	void SourcePause(AAudioSourceComponent* source, int delay = 0);
	/// The first person delays ms according to the buffer stop
	void SourceStop(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer stop
	void SourceStop(AAudioSourceComponent* source, int delay = 0);
	/// The first person delays ms according to the buffer rewind
	void SourceRewind(unsigned source, int delay = 0);
	/// The first person delays ms according to the buffer rewind
	void SourceRewind(AAudioSourceComponent* source, int delay = 0);
	/// Set pitch of source address
	void SetPitch(unsigned source, float val);
	/// Set gain of source address
	void SetGain(unsigned source, float val);
	/// Set vel of source address
	void SetVel(unsigned source, TVector3F vel);
	/// Set get state of source
	EAudioSourceState::Type GetState(unsigned source);
	/// Set get state of source
	EAudioSourceState::Type GetState(AAudioSourceComponent* source);
	/// Get source of index
	const AAudioSourceComponent* GetSource(unsigned index);
	/// Return sources
	THashMap<unsigned, AAudioSourceComponent*>& Sources(AAudioSourceComponent* source) { return _sources; }
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
	AAudioListenerComponent* _listener;
	/// Source array of hash
	THashMap<unsigned, AAudioSourceComponent*> _sources;
};


}