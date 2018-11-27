#include "TypeThread.h"


#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace Auto3D {

#ifdef _WIN32

static DWORD WINAPI ThreadFunctionStatic(void* data)
{
	tThread* thread = static_cast<tThread*>(data);
	thread->ThreadFunction();
	return 0;
}

#else

static void* ThreadFunctionStatic(void* data)
{
	auto* thread = static_cast<Thread*>(data);
	thread->ThreadFunction();
	pthread_exit((void*)nullptr);
	return nullptr;
}

#endif

ThreadID tThread::mainThreadID;

tThread::tThread()
	: _handle(nullptr)
	, _shouldRun(false)
{
}


tThread::~tThread()
{
	Stop();
}

bool tThread::Run()
{
	// Check if already running
	if (_handle)
		return false;

	_shouldRun = true;
#ifdef _WIN32
	_handle = CreateThread(nullptr, 0, ThreadFunctionStatic, this, 0, nullptr);
#else
	_handle = new pthread_t;
	pthread_attr_t type;
	pthread_attr_init(&type);
	pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);
	pthread_create((pthread_t*)_handle, &type, ThreadFunctionStatic, this);
#endif
	return _handle != nullptr;
}

void tThread::Stop()
{
	if (!_handle)
		return;

	_shouldRun = false;
#ifdef _WIN32
	WaitForSingleObject((HANDLE)_handle, INFINITE);
	CloseHandle((HANDLE)_handle);
#else
	auto* thread = (pthread_t*)_handle;
	if (thread)
		pthread_join(*thread, nullptr);
	delete thread;
#endif
	_handle = nullptr;
}


void tThread::SetPriority(int priority)
{
#ifdef _WIN32
	if (_handle)
		SetThreadPriority((HANDLE)_handle, priority);
#elif defined(__linux__) && !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
	auto* thread = (pthread_t*)handle_;
	if (thread)
		pthread_setschedprio(*thread, priority);
#endif
}

void tThread::SetMainThread()
{
	mainThreadID = GetCurrentThreadID();
}

ThreadID tThread::GetCurrentThreadID()
{
#ifdef _WIN32
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

bool tThread::IsMainThread()
{
	return GetCurrentThreadID() == mainThreadID;
}

}