#pragma once
#include "Resource/Resource.h"

namespace Auto3D 
{

class ASound : public AResource
{
	REGISTER_OBJECT(ASound, AResource)
public:
	ASound();

	~ASound();
	/// Register object factory
	static void RegisterObject();
	/// Load resource from stream.May be called from a worker thread.Return true if successful
	bool BeginLoad(FStream& source)override;
	/// Load raw sound data.
	bool LoadRaw(FStream& source);
	/// Load WAV format sound data.
	bool LoadWav(FStream& source);
	/// Load Ogg Vorbis format sound data. Does not decode at load, but will rather be decoded while playing.
	bool LoadOggVorbis(FStream& source);
	/// Set data with void*
	void SetData(void* _data, unsigned dataSize);
	/// Set size with long
	void SetSize(unsigned dataSize);
	/// Set uncompressed sound data format.
	void SetFormat(unsigned frequency, bool sixteenBit, bool stereo);
	/// Set loop on/off. If loop is enabled, sets the full sound as loop range.
	void SetLooped(bool enable);
	/// Define loop.
	void SetLoop(unsigned repeatOffset, unsigned endOffset);
	/// Get buffer data with void*
	TSharedArrayPtr<signed char> GetData() { return _data; }
	/// Return sound data start.
	signed char* GetStart() const { return _data.Get(); }
	/// Return loop start.
	signed char* GetRepeat() const { return _repeat; }
	/// Return sound data end.
	signed char* GetEnd() const { return _end; }
	/// Return length in seconds.
	float GetLength() const;
	/// Return total sound data size.
	unsigned GetDataSize() const { return _dataSize; }
	/// Return sample size.
	unsigned GetSampleSize() const;
	/// Return default frequency as a float.
	float GetFrequency() const { return (float)_frequency; }
	/// Return default frequency as an integer.
	unsigned GetIntFrequency() const { return _frequency; }
	/// Return whether is looped.
	bool IsLooped() const { return _looped; }
	/// Return whether data is sixteen bit.
	bool IsSixteenBit() const { return _sixteenBit; }
	/// Return whether data is stereo.
	bool IsStereo() const { return _stereo; }
	/// Return whether is compressed.
	bool IsCompressed() const { return _compressed; }
	/// Fix interpolation by copying data from loop start to loop end (looped), or adding silence (oneshot.) Called internally, does not normally need to be called, unless the sound data is modified manually on the fly.
	void FixInterpolation();
private:

	/// buffer data
	TSharedArrayPtr<signed char> _data;
	/// Loop start.
	signed char* _repeat;
	/// ASound data end.
	signed char* _end;
	/// ASound data size in bytes.
	unsigned _dataSize;
	/// Default frequency.
	unsigned _frequency;
	/// Looped flag.
	bool _looped;
	/// Sixteen bit flag.
	bool _sixteenBit;
	/// Stereo flag.
	bool _stereo;
	/// Compressed flag.
	bool _compressed;
	/// Compressed sound length.
	float _compressedLength;
};

}