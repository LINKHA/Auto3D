#include "Sound.h"
#include "IO/Stream.h"
#include "Debug/Log.h"
#include "IO/FileSystem.h"
#include "Resource/ResourceCache.h"

#include <stb_vorbis.h>

#include "Debug/DebugNew.h"

namespace Auto3D {

/// WAV format header.
struct WavHeader
{
	unsigned char _riffText[4];
	unsigned _totalLength;
	unsigned char _waveText[4];
	unsigned char _formatText[4];
	unsigned _formatLength;
	unsigned short _format;
	unsigned short _channels;
	unsigned _frequency;
	unsigned _avgBytes;
	unsigned short _blockAlign;
	unsigned short _bits;
	unsigned char _dataText[4];
	unsigned _dataLength;
};

static const unsigned IP_SAFETY = 4;

OSound::OSound()
{
}


OSound::~OSound()
{

}

bool OSound::BeginLoad(const FString& pathName)
{
	SPtr<FStream> stream(GResourceModule::Get().OpenResource(pathName));
	FStream& source = *stream;
	bool success;
	if (Extension(pathName) == ".ogg")
		success = LoadOggVorbis(source);
	else if (Extension(pathName) == ".wav")
		success = LoadWav(source);
	else
		success = LoadRaw(source);
	return success;
}


bool OSound::LoadRaw(FStream& source)
{
	unsigned dataSize = source.Size();
	SetSize(dataSize);
	return source.Read(_data.Get(), dataSize) == dataSize;
	return false;
}

bool OSound::LoadWav(FStream& source)
{
	WavHeader header{};

	// Try to open
	memset(&header, 0, sizeof header);
	source.Read(&header._riffText, 4);
	header._totalLength = source.Read<unsigned>();
	source.Read(&header._waveText, 4);

	if (memcmp("RIFF", header._riffText, 4) != 0 || memcmp("WAVE", header._waveText, 4) != 0)
	{
		ErrorString("Could not read WAV data from " + source.GetName());
		return false;
	}

	// Search for the FORMAT chunk
	for (;;)
	{
		source.Read(&header._formatText, 4);
		header._formatLength = source.Read<unsigned>();
		if (!memcmp("fmt ", &header._formatText, 4))
			break;

		source.Seek(source.Position() + header._formatLength);
		if (!header._formatLength || source.Position() >= source.Size())
		{
			ErrorString("Could not read WAV data from " + source.GetName());
			return false;
		}
	}

	// Read the FORMAT chunk
	header._format = source.Read<unsigned short>();
	header._channels = source.Read<unsigned short>();
	header._frequency = source.Read<unsigned>();
	header._avgBytes = source.Read<unsigned>();
	header._blockAlign = source.Read<unsigned short>();
	header._bits = source.Read<unsigned short>();

	// Skip data if the format chunk was bigger than what we use
	source.Seek(source.Position() + header._formatLength - 16);

	// Check for correct format
	if (header._format != 1)
	{
		ErrorString("Could not read WAV data from " + source.GetName());
		return false;
	}

	// Search for the DATA chunk
	for (;;)
	{
		source.Read(&header._dataText, 4);
		header._dataLength = source.Read<unsigned>();
		if (!memcmp("data", &header._dataText, 4))
			break;

		source.Seek(source.Position() + header._dataLength);
		if (!header._dataLength || source.Position() >= source.Size())
		{
			ErrorString("Could not read WAV data from " + source.GetName());
			return false;
		}
	}

	// Allocate sound and load audio data
	unsigned length = header._dataLength;
	SetSize(length);
	SetFormat(header._frequency, header._bits == 16, header._channels == 2);
	source.Read(_data.Get(), length);

	// Convert 8-bit audio to signed
	if (!_sixteenBit)
	{
		for (unsigned i = 0; i < length; ++i)
			_data[i] -= 128;
	}

	return true;
}

bool OSound::LoadOggVorbis(FStream& source)
{
	unsigned dataSize = source.Size();
	TSharedArrayPtr<signed char> data(new signed char[dataSize]);
	source.Read(data.Get(), dataSize);

	// Check for validity of data
	int error;
	stb_vorbis* vorbis = stb_vorbis_open_memory((unsigned char*)data.Get(), dataSize, &error, nullptr);
	if (!vorbis)
	{
		ErrorString("Could not read Ogg Vorbis data from " + source.GetName());
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

void OSound::SetData(void* data, unsigned dataSize)
{
	if (!dataSize)
		return;

	SetSize(dataSize);
	memcpy(_data.Get(), data, dataSize);
}

void OSound::SetSize(unsigned dataSize)
{
	if (!dataSize)
		return;

	_data = new signed char[dataSize + IP_SAFETY];
	_dataSize = dataSize;
	_compressed = false;
	SetLooped(false);

	SetMemoryUse(dataSize + IP_SAFETY);
}

void OSound::SetFormat(unsigned frequency, bool sixteenBit, bool stereo)
{
	_frequency = frequency;
	_sixteenBit = sixteenBit;
	_stereo = stereo;
	_compressed = false;
}

void OSound::SetLooped(bool enable)
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

void OSound::SetLoop(unsigned repeatOffset, unsigned endOffset)
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

float OSound::GetLength() const
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

unsigned OSound::GetSampleSize() const
{
	unsigned _size = 1;
	if (_sixteenBit)
		_size <<= 1;
	if (_stereo)
		_size <<= 1;
	return _size;
}

void OSound::FixInterpolation()
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