#include "AudioListener.h"
#include "Audio.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace Auto3D
{

AudioListener::AudioListener() :
	_listenerVel(0.0f, 0.0f, 0.0f),
	_listenerOriAt(0.0f, 0.0f, -1.0f),
	_listenerOriUp(0.0f, 1.0f, 0.0f)
{
	_audio = Subsystem<Audio>();
	ALfloat listenerVelArray[] = { _listenerVel._x, _listenerVel._y, _listenerVel._z };

	// Listener speed
	ALfloat ListenerOriArray[] = {
		_listenerOriAt._x, _listenerOriAt._y, _listenerOriAt._z,
		_listenerOriUp._x , _listenerOriUp._y ,_listenerOriUp._z };

	alListener3f(AL_POSITION, GetPosition()._x, GetPosition()._y, GetPosition()._z);
	alListenerfv(AL_VELOCITY, listenerVelArray);
	alListenerfv(AL_ORIENTATION, ListenerOriArray);
}
AudioListener::~AudioListener()
{

}

void AudioListener::RegisterObject()
{
	RegisterFactory<AudioListener>();
}

void AudioListener::Update()
{
	Vector3F vec = GetPosition();
	ALfloat listenerVelArray[] = { _listenerVel._x, _listenerVel._y, _listenerVel._z };

	// Listener speed
	ALfloat ListenerOriArray[] = {
		_listenerOriAt._x, _listenerOriAt._y, _listenerOriAt._z,
		_listenerOriUp._x , _listenerOriUp._y ,_listenerOriUp._z };

	alListener3f(AL_POSITION, vec._x, vec._y, vec._z);
	alListenerfv(AL_VELOCITY, listenerVelArray);
	alListenerfv(AL_ORIENTATION, ListenerOriArray);
}



}