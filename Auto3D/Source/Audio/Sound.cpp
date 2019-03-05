#include "Sound.h"
#include "../IO/Stream.h"
#include "../Debug/Log.h"
#include "../IO/FileSystem.h"

#include <stb_vorbis.h>

#include "../Debug/DebugNew.h"

namespace Auto3D {

/// WAV format header.
struct WavHeader
{
	unsigned char riffText_[4];
	unsigned totalLength_;
	unsigned char waveText_[4];
	unsigned char formatText_[4];
	unsigned formatLength_;
	unsigned short format_;
	unsigned short channels_;
	unsigned _frequency;
	unsigned avgBytes_;
	unsigned short blockAlign_;
	unsigned short bits_;
	unsigned char dataText_[4];
	unsigned dataLength_;
};

static const unsigned IP_SAFETY = 4;

Sound::Sound()
{
}


Sound::~Sound()
{

}

void Sound::RegisterObject()
{
	RegisterFactory<Sound>();
}

bool Sound::BeginLoad(Stream& source)
{
	bool success;
	if (Extension(source.Name()) == ".ogg")
		success = LoadOggVorbis(source);
	else if (Extension(source.Name()) == ".wav")
		success = LoadWav(source);
	else
		success = LoadRaw(source);
	return success;
}


bool Sound::LoadRaw(Stream& source)
{
	unsigned dataSize = source.Size();
	SetSize(dataSize);
	return source.Read(_data.Get(), dataSize) == dataSize;
	return false;
}

bool Sound::LoadWav(Stream& source)
{
	WavHeader header{};

	// Try to open
	memset(&header, 0, sizeof header);
	source.Read(&header.riffText_, 4);
	header.totalLength_ = source.Read<unsigned>();
	source.Read(&header.waveText_, 4);

	if (memcmp("RIFF", header.riffText_, 4) != 0 || memcmp("WAVE", header.waveText_, 4) != 0)
	{
		ErrorString("Could not read WAV data from " + source.Name());
		return false;
	}

	// Search for the FORMAT chunk
	for (;;)
	{
		source.Read(&header.formatText_, 4);
		header.formatLength_ = source.Read<unsigned>();
		if (!memcmp("fmt ", &header.formatText_, 4))
			break;

		source.Seek(source.Position() + header.formatLength_);
		if (!header.formatLength_ || source.Position() >= source.Size())
		{
			ErrorString("Could not read WAV data from " + source.Name());
			return false;
		}
	}

	// Read the FORMAT chunk
	header.format_ = source.Read<unsigned short>();
	header.channels_ = source.Read<unsigned short>();
	header._frequency = source.Read<unsigned>();
	header.avgBytes_ = source.Read<unsigned>();
	header.blockAlign_ = source.Read<unsigned short>();
	header.bits_ = source.Read<unsigned short>();

	// Skip data if the format chunk was bigger than what we use
	source.Seek(source.Position() + header.formatLength_ - 16);

	// Check for correct format
	if (header.format_ != 1)
	{
		ErrorString("Could not read WAV data from " + source.Name());
		return false;
	}

	// Search for the DATA chunk
	for (;;)
	{
		source.Read(&header.dataText_, 4);
		header.dataLength_ = source.Read<unsigned>();
		if (!memcmp("data", &header.dataText_, 4))
			break;

		source.Seek(source.Position() + header.dataLength_);
		if (!header.dataLength_ || source.Position() >= source.Size())
		{
			ErrorString("Could not read WAV data from " + source.Name());
			return false;
		}
	}

	// Allocate sound and load audio data
	unsigned length = header.dataLength_;
	SetSize(length);
	SetFormat(header._frequency, header.bits_ == 16, header.channels_ == 2);
	source.Read(_data.Get(), length);

	// Convert 8-bit audio to signed
	if (!_sixteenBit)
	{
		for (unsigned i = 0; i < length; ++i)
			_data[i] -= 128;
	}

	return true;
}

bool Sound::LoadOggVorbis(Stream& source)
{
	unsigned dataSize = source.Size();
	SharedArrayPtr<signed char> data(new signed char[dataSize]);
	source.Read(data.Get(), dataSize);

	// Check for validity of data
	int error;
	stb_vorbis* vorbis = stb_vorbis_open_memory((unsigned char*)data.Get(), dataSize, &error, nullptr);
	if (!vorbis)
	{
		ErrorString("Could not read Ogg Vorbis data from " + source.Name());
		return false;
	}

	// Store length, frequency and stereo flag
	stb_vorbis_info info = stb_vorbis_get_info(vorbis);
	_compressedLength = stb_vorbis_stream_length_in_seconds(vorbis);
	_frequency = info.sample_rate;
	_stereo = info.channels > 1;
	stb_vorbis_close(vorbis);

	_data = data;
	_dataSize = dataSize;
	_sixteenBit = true;
	_compressed = true;

	SetMemoryUse(dataSize);
	return true;
}

void Sound::SetData(void* data, unsigned dataSize)
{
	if (!dataSize)
		return;

	SetSize(dataSize);
	memcpy(_data.Get(), data, dataSize);
}

void Sound::SetSize(unsigned dataSize)
{
	if (!dataSize)
		return;

	_data = new signed char[dataSize + IP_SAFETY];
	_dataSize = dataSize;
	_compressed = false;
	SetLooped(false);

	SetMemoryUse(dataSize + IP_SAFETY);
}

void Sound::SetFormat(unsigned frequency, bool sixteenBit, bool stereo)
{
	_frequency = frequency;
	_sixteenBit = sixteenBit;
	_stereo = stereo;
	_compressed = false;
}

void Sound::SetLooped(bool enable)
{
	if (enable)
		SetLoop(0, _dataSize);
	else
	{
		if (!_compressed)
		{
			_end = _data.Get() + _dataSize;
			_looped = false;

			FixInterpolation();
		}
		else
			_looped = false;
	}
}

void Sound::SetLoop(unsigned repeatOffset, unsigned endOffset)
{
	if (!_compressed)
	{
		if (repeatOffset > _dataSize)
			repeatOffset = _dataSize;
		if (endOffset > _dataSize)
			endOffset = _dataSize;

		// Align repeat and end on sample boundaries
		int sampleSize = GetSampleSize();
		repeatOffset &= -sampleSize;
		endOffset &= -sampleSize;

		_repeat = _data.Get() + repeatOffset;
		_end = _data.Get() + endOffset;
		_looped = true;

		FixInterpolation();
	}
	else
		_looped = true;
}

float Sound::GetLength() const
{
	if (!_compressed)
	{
		if (!_frequency)
			return 0.0f;
		else
			return ((float)_dataSize) / GetSampleSize() / _frequency;
	}
	else
		return _compressedLength;
}

unsigned Sound::GetSampleSize() const
{
	unsigned size = 1;
	if (_sixteenBit)
		size <<= 1;
	if (_stereo)
		size <<= 1;
	return size;
}

void Sound::FixInterpolation()
{
	if (!_data || _compressed)
		return;

	// If looped, copy loop start to loop end. If oneshot, insert silence to end
	if (_looped)
	{
		for (unsigned i = 0; i < IP_SAFETY; ++i)
			_end[i] = _repeat[i];
	}
	else
	{
		for (unsigned i = 0; i < IP_SAFETY; ++i)
			_end[i] = 0;
	}
}


}