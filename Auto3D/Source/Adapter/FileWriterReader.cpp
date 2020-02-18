#include "Adapter/FileWriterReader.h"
#include "Container/String.h"
#include "Application.h"

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/allocator.h>


namespace Auto3D
{
bx::FileReaderI* FDefaultFileWriterReader::_fileReader = NULL;
bx::FileWriterI* FDefaultFileWriterReader::_fileWriter = NULL;
bx::AllocatorI* FDefaultFileWriterReader::_allocator = GetDefaultAllocator();

class FileReader : public bx::FileReader
{
	typedef bx::FileReader super;

public:
	virtual bool open(const bx::FilePath& filePath, bx::Error* err) override
	{
		FString curFilePath(GApplication::_currentDir.CString());
		curFilePath.Append(filePath.getCPtr());
		return super::open(curFilePath.CString(), err);
	}
};

class FileWriter : public bx::FileWriter
{
	typedef bx::FileWriter super;

public:
	virtual bool open(const bx::FilePath& filePath, bool append, bx::Error* err) override
	{
		FString curFilePath(GApplication::_currentDir.CString());
		curFilePath.Append(filePath.getCPtr());
		return super::open(curFilePath.CString(), append, err);
	}
};

void FDefaultFileWriterReader::Reset()
{
	_fileReader = NULL;
	_fileWriter = NULL;
	_allocator = GetDefaultAllocator();
}

bx::AllocatorI* FDefaultFileWriterReader::GetDefaultAllocator()
{
	BX_PRAGMA_DIAGNOSTIC_PUSH();
	BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
	BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
	static bx::DefaultAllocator s_allocator;
	return &s_allocator;
	BX_PRAGMA_DIAGNOSTIC_POP();
}


bx::AllocatorI* FDefaultFileWriterReader::GetAllocator()
{
	if (NULL == _allocator)
	{
		_allocator = GetDefaultAllocator();
	}

	return _allocator;
}

bx::FileReaderI* FDefaultFileWriterReader::GetFileReader()
{
	if (NULL == _fileReader)
	{
		_fileReader = BX_NEW(GetAllocator(), FileReader);
	}
	return _fileReader;
}

bx::FileWriterI* FDefaultFileWriterReader::GetFileWriter()
{
	if (NULL == _fileWriter)
	{
		_fileWriter = BX_NEW(GetAllocator(), FileWriter);
	}
	return _fileWriter;
}

void FDefaultFileWriterReader::Release()
{
	BX_DELETE(_allocator, _fileReader);
	_fileReader = NULL;

	BX_DELETE(_allocator, _fileWriter);
	_fileWriter = NULL;
}

}