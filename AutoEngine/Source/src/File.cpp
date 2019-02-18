#include "File.h"
#include "FileSystem.h"
#include "MathBase.h"
#include "MemoryBuffer.h"
#include "AutoZIP.h"
#include "NewDef.h"

namespace Auto3D {

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4996)
#endif

#ifdef _WIN32
static const wchar_t* openMode[] =
{
	L"rb",
	L"wb",
	L"r+b",
	L"w+b"
};
#else
static const char* openMode[] =
{
	"rb",
	"wb",
	"r+b",
	"w+b"
};
#endif

static const unsigned SKIP_BUFFER_SIZE = 1024;

File::File(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _mode(FileMode::Read)
	, _handle(nullptr)
	, _readBufferOffset(0)
	, _readBufferSize(0)
	, _offset(0)
	, _checksum(0)
	, _compressed(false)
	, _readSyncNeeded(false)
	, _writeSyncNeeded(false)
{}

File::File(SharedPtr<Ambient> ambient, const STRING& fileName, FileMode mode)
	: Super(ambient)
	, _mode(FileMode::Read)
	, _handle(nullptr)
	, _readBufferOffset(0)
	, _readBufferSize(0)
	, _offset(0)
	, _checksum(0)
	, _compressed(false)
	, _readSyncNeeded(false)
	, _writeSyncNeeded(false)
{
	Open(fileName, mode);
}

File::~File() 
{
}

unsigned File::Read(void* dest, unsigned size)
{
	if (!IsOpen())
	{
		// If file not open, do not log the error further here to prevent spamming the stderr stream
		return 0;
	}

	if (_mode == FileMode::Write)
	{
		WarningString("File not opened for reading");
		return 0;
	}

	if (size + _position > _size)
		size = _size - _position;
	if (!size)
		return 0;
	if (_compressed)
	{
		unsigned sizeLeft = size;
		auto* destPtr = (unsigned char*)dest;

		while (sizeLeft)
		{
			if (!_readBuffer || _readBufferOffset >= _readBufferSize)
			{
				unsigned char blockHeaderBytes[4];
				readInternal(blockHeaderBytes, sizeof blockHeaderBytes);

				MemoryBuffer blockHeader(&blockHeaderBytes[0], sizeof blockHeaderBytes);
				unsigned unpackedSize = blockHeader.ReadUShort();
				unsigned packedSize = blockHeader.ReadUShort();

				if (!_readBuffer)
				{
					_readBuffer = SharedArrayPtr<unsigned char>(new unsigned char[unpackedSize]);
					_inputBuffer = SharedArrayPtr<unsigned char>(new unsigned char[LZ4_compressBound(unpackedSize)]);
				}

				/// \todo Handle errors
				readInternal(_inputBuffer.get(), packedSize);
				LZ4_decompress_fast((const char*)_inputBuffer.get(), (char*)_readBuffer.get(), unpackedSize);

				_readBufferSize = unpackedSize;
				_readBufferOffset = 0;
			}

			unsigned copySize = min((_readBufferSize - _readBufferOffset), sizeLeft);
			memcpy(destPtr, _readBuffer.get() + _readBufferOffset, copySize);
			destPtr += copySize;
			sizeLeft -= copySize;
			_readBufferOffset += copySize;
			_position += copySize;
		}

		return size;
	}

	// Need to reassign the position due to internal buffering when transitioning from writing to reading
	if (_readSyncNeeded)
	{
		seekInternal(_position + _offset);
		_readSyncNeeded = false;
	}

	if (!readInternal(dest, size))
	{
		// Return to the position where the read began
		seekInternal(_position + _offset);
		ErrorString("Error while reading from file " + GetName());
		return 0;
	}

	_writeSyncNeeded = true;
	_position += size;
	return size;
}

unsigned File::Seek(unsigned position)
{
	if (!IsOpen())
	{
		// If file not open, do not log the error further here to prevent spamming the stderr stream
		return 0;
	}

	// Allow sparse seeks if writing
	if (_mode == FileMode::Read && position > _size)
		position = _size;

	if (_compressed)
	{
		// Start over from the beginning
		if (position == 0)
		{
			_position = 0;
			_readBufferOffset = 0;
			_readBufferSize = 0;
			seekInternal(_offset);
		}
		// Skip bytes
		else if (position >= _position)
		{
			unsigned char skipBuffer[SKIP_BUFFER_SIZE];
			while (position > _position)
				Read(skipBuffer, min(position - _position, SKIP_BUFFER_SIZE));
		}
		else
			WarningString("Seeking backward in a compressed file is not supported");

		return _position;
	}

	seekInternal(position + _offset);
	_position = position;
	_readSyncNeeded = false;
	_writeSyncNeeded = false;
	return _position;
}

unsigned File::Write(const void* data, unsigned size)
{
	if (!IsOpen())
	{
		// If file not open, do not log the error further here to prevent spamming the stderr stream
		return 0;
	}

	if (_mode == FileMode::Read)
	{
		LogString("File not opened for writing");
		return 0;
	}

	if (!size)
		return 0;

	// Need to reassign the position due to internal buffering when transitioning from reading to writing
	if (_writeSyncNeeded)
	{
		fseek((FILE*)_handle, (long)_position + _offset, SEEK_SET);
		_writeSyncNeeded = false;
	}

	if (fwrite(data, size, 1, (FILE*)_handle) != 1)
	{
		// Return to the position where the write began
		fseek((FILE*)_handle, (long)_position + _offset, SEEK_SET);
		ErrorString("Error while writing to file " + GetName());
		return 0;
	}

	_readSyncNeeded = true;
	_position += size;
	if (_position > _size)
		_size = _position;

	return size;
}

bool File::Open(const STRING& fileName, FileMode mode)
{
	return openInternal(fileName, mode);
}

void File::Close()
{
	_readBuffer.reset();
	_inputBuffer.reset();
	if (_handle)
	{
		fclose((FILE*)_handle);
		_handle = nullptr;
		_position = 0;
		_size = 0;
		_offset = 0;
		_checksum = 0;
	}
}

void File::Flush()
{
	if (_handle)
		fflush((FILE*)_handle);
}
void File::SetName(const STRING& name)
{
	_fileName = name;
}

bool File::IsOpen() const
{
	return _handle != nullptr;
}

bool File::openInternal(const STRING& fileName, FileMode mode, bool fromPackage)
{
	Close();

	_compressed = false;
	_readSyncNeeded = false;
	_writeSyncNeeded = false;
	auto fileSystem = GetSubSystem<FileSystem>();

	if (fileSystem && !fileSystem->CheckAccess(GetPath(fileName)))
	{
		AutoCout << "Access denied to" << fileName.CStr() << AutoEndl;
		return false;
	}

	if (fileName.Empty())
	{
		LogString("Could not open file with empty name");
		return false;
	}
#ifdef _WIN32
	_handle = _wfopen(WSTRING(GetNativePath(fileName)).CStr(), openMode[static_cast<int>(mode)]);
#else
	_handle = fopen(GetNativePath(fileName).CStr(), openMode[static_cast<int>(mode)]);
#endif
	// If file did not exist in readwrite mode, retry with write-update mode
	if (mode == FileMode::ReadWrite && !_handle)
	{
#ifdef _WIN32
		_handle = _wfopen(WSTRING(GetNativePath(fileName)).CStr(), openMode[static_cast<int>(mode) + 1]);
#else
		_handle = fopen(GetNativePath(fileName).CStr(), openMode[static_cast<int>(mode) + 1]);
#endif
	}

	if (!_handle)
	{
		AutoCout << "Could not open file" << fileName.CStr() << AutoEndl;
		return false;
	}

	if (!fromPackage)
	{
		fseek((FILE*)_handle, 0, SEEK_END);
		long size = ftell((FILE*)_handle);
		fseek((FILE*)_handle, 0, SEEK_SET);
		if (size > M_MAX_UNSIGNED)
		{
			AutoCout << "Could not open file" << fileName.CStr() << "which is larger than 4GB" << AutoEndl;
			Close();
			_size = 0;
			return false;
		}
		_size = (unsigned)size;
		_offset = 0;
	}

	_fileName = fileName;
	_mode = mode;
	_position = 0;
	_checksum = 0;

	return true;
}

bool File::readInternal(void* dest, unsigned size)
{
	return fread(dest, size, 1, (FILE*)_handle) == 1;
}

void File::seekInternal(unsigned newPosition)
{
	fseek((FILE*)_handle, newPosition, SEEK_SET);
}

}