#pragma once
#include "../Resource/Resource.h"

namespace Auto3D 
{

class Sound : public Resource
{
	REGISTER_OBJECT_CLASS(Sound, Resource)
public:
	Sound();

	~Sound();
	/**
	* @brief : Register object factory
	*/
	static void RegisterObject();
	/*
	* @brief : Load resource from stream.May be called from a worker thread.Return true if successful
	*/
	bool BeginLoad(Stream& source)override;
	/// Load raw sound data.
	bool LoadRaw(Stream& source);
	/// Load WAV format sound data.
	bool LoadWav(Stream& source);
	/// Load Ogg Vorbis format sound data. Does not decode at load, but will rather be decoded while playing.
	bool LoadOggVorbis(Stream& source);
	/**
	* @brief : Set data with void*
	*/
	void SetData(void* data) { _data = data; }
	/**
	* @brief : Set size with long
	*/
	void SetSize(long size) { _size = size; }
	/**
	* @brief : Get buffer data with void*
	*/
	void* GetData() { return _data; }
	/**
	* @brief : Get buffer size with long
	*/
	long GetSize() { return _size; }
private:
	/// buffer data
	void* _data;
	/// buffer size
	long _size;
};

}