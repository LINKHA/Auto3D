#pragma once
#include "Auto.h"


#ifndef _WIN32
#	include <pthread.h>
	using ThreadID = pthread_t;
#else
	using ThreadID = unsigned;
#endif

namespace Auto3D {

class tThread
{
public:
	tThread();

	~tThread();

	virtual void ThreadFunction() = 0;
	/**
	* @brief : Start running the thread. Return true if successful, or false
	*	if already running or if can not create the thread
	*/
	bool Run();
	/**
	* @brief :  Set the running flag to false and wait for the thread to finish
	*/
	void Stop();
	/**
	* @brief : Set thread priority. The thread must have been started first
	*/
	void SetPriority(int priority);
	/**
	* @brief : Return whether thread exists
	*/
	bool IsStarted() const { return _handle != nullptr; }
	/**
	* @brief : Set the current thread as the main thread
	*/
	static void SetMainThread();
	/**
	* @brief : Return the current thread's ID
	*/
	static ThreadID GetCurrentThreadID();
	/**
	* @brief : Return whether is executing in the main thread
	*/
	static bool IsMainThread();

protected:
	/// thread handle
	void* _handle;
	/// running flag
	volatile bool _shouldRun;
	/// main thread's thread ID
	static ThreadID mainThreadID;
};
}