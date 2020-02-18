#pragma once
namespace bx { struct FileReaderI; struct FileWriterI; struct AllocatorI; }

namespace Auto3D
{

struct FDefaultFileWriterReader
{
	static void Reset();
	static bx::AllocatorI* GetDefaultAllocator();
	static bx::FileReaderI* GetFileReader();
	static bx::FileWriterI* GetFileWriter();
	static bx::AllocatorI*  GetAllocator();
	static void Release();

private:
	static bx::FileReaderI* _fileReader;
	static bx::FileWriterI* _fileWriter;
	static bx::AllocatorI* _allocator;
};

}


