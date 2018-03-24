#ifndef FILE_H_
#define FILE_H_

#include "Auto.h"

class File
{
	int							m_Position;
	std::string					m_Path;
	bool						m_Open;
	FILE*						m_File;
	HANDLE						m_FileHandle;
	_VECTOR(unsigned char)		m_Data;

public:
	File();
	virtual ~File();

	bool Open(const std::string& path, Permission perm, AutoBehavior behavior = kNormalBehavior);
	bool Close();

	int Read(void* buffer, int size);
	int Read(int position, void* buffer, int size);

	bool Write(const void* buffer, int size);
	bool Write(int pos, const void* buffer, int size);
	bool SetFileLength(int size);
	int GetFileLength();
	int GetPosition() const { return m_Position; }
};

#endif // !FILE_H_