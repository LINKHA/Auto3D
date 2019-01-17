#pragma once

#include "Auto.h"
#include "Object.h"
#include "AbstractFile.h"

namespace Auto3D {


/**
* File open mode
*/
enum class FileMode
{
	Read = 0,
	Write,
	ReadWrite
};

class File : public Object, public AbstractFile
{
	REGISTER_OBJECT_CLASS(File, Object)
public:
	/**
	* @brief : Construct
	*/
	explicit File(SharedPtr<Ambient> ambient);
	/**
	* @brief : Construct and open a filesystem file
	*/
	File(SharedPtr<Ambient> ambient, const STRING& fileName, FileMode mode = FileMode::Read);
	/**
	* @brief : Read bytes from the file. Return number of bytes actually read
	*/
	unsigned Read(void* dest, unsigned size) override;
	/**
	* @brief : Set position from the beginning of the file
	*/
	unsigned Seek(unsigned position) override;
	/**
	* @brief : Write bytes to the file. Return number of bytes actually written
	*/
	unsigned Write(const void* data, unsigned size) override;
	/**
	* @brief : Return the file name
	*/
	const STRING& GetName() const override { return _fileName; }
	/**
	* @brief : Open a filesystem file. Return true if successful
	*/
	bool Open(const STRING& fileName, FileMode mode = FileMode::Read);
	/**
	* @brief : Close the file
	*/
	void Close();
	/**
	* @brief : Flush any buffered output to the file
	*/
	void Flush();
	/**
	* @brief : Change the file name. Used by the resource system
	*/
	void SetName(const STRING& name);
	/**
	* @brief : Return the open mode
	*/
	FileMode GetMode() const { return _mode; }
	/**
	* @brief : Return whether is open
	*/
	bool IsOpen() const;
	/**
	* @brief : Return the file handle
	*/
	void* GetHandle() const { return _handle; }
	/**
	* @brief : Return whether the file originates from a package
	*/
	bool IsPackaged() const { return _offset != 0; }
private:
	/**
	* @brief : Open file internally using either C standard IO functions or SDL RWops for 
	*	Android asset files. Return true if successful
	*/
	bool openInternal(const STRING& fileName, FileMode mode, bool fromPackage = false);
	/**
	* @brief :  Perform the file read internally using either C standard IO functions or 
	*	SDL RWops for Android asset files. Return true if successful. This does not handle 
	*	compressed package file reading
	*/
	bool readInternal(void* dest, unsigned size);
	/**
	* @brief : Seek in file internally using either C standard IO 
	*	functions or SDL RWops for Android asset files
	*/
	void seekInternal(unsigned newPosition);
private:
	/// file name
	STRING _fileName;
	/// open mode
	FileMode _mode;
	/// file handle
	void* _handle;
	/// read buffer for Android asset or compressed file loading
	SharedArrayPtr<unsigned char> _readBuffer;
	/// decompression input buffer for compressed file loading
	SharedArrayPtr<unsigned char> _inputBuffer;
	/// read buffer position
	unsigned _readBufferOffset;
	/// bytes in the current read buffer
	unsigned _readBufferSize;
	/// start position within a package file, 0 for regular files
	unsigned _offset;
	/// content checksum
	unsigned _checksum;
	/// compression flag
	bool _compressed;
	/// synchronization needed before read -flag
	bool _readSyncNeeded;
	/// synchronization needed before write -flag
	bool _writeSyncNeeded;
};

}