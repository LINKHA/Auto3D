#include "Adapter/FileWriterReader.h"
#include "Container/String.h"
#include "Application.h"

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/allocator.h>


namespace Auto3D
{
bx::FileReaderI* FDefaultFileWriterReader::s_fileReader = NULL;
bx::FileWriterI* FDefaultFileWriterReader::s_fileWriter = NULL;
bx::AllocatorI* FDefaultFileWriterReader::s_allocator = GetDefaultAllocator();

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
	s_fileReader = NULL;
	s_fileWriter = NULL;
	s_allocator = GetDefaultAllocator();
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
	if (NULL == s_allocator)
	{
		s_allocator = GetDefaultAllocator();
	}

	return s_allocator;
}

bx::FileReaderI* FDefaultFileWriterReader::GetFileReader()
{
	if (NULL == s_fileReader)
	{
		s_fileReader = BX_NEW(GetAllocator(), FileReader);
	}
	return s_fileReader;
}

bx::FileWriterI* FDefaultFileWriterReader::GetFileWriter()
{
	if (NULL == s_fileWriter)
	{
		s_fileWriter = BX_NEW(GetAllocator(), FileWriter);
	}
	return s_fileWriter;
}

void FDefaultFileWriterReader::Release()
{
	BX_DELETE(s_allocator, s_fileReader);
	s_fileReader = NULL;

	BX_DELETE(s_allocator, s_fileWriter);
	s_fileWriter = NULL;
}

}