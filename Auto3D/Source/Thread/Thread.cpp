#include "Thread.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "Debug/DebugNew.h"

namespace Auto3D
{

#ifdef _WIN32
DWORD WINAPI ThreadFunctionStatic(void* _data)
{
    FThread* thread = static_cast<FThread*>(_data);
    thread->ThreadFunction();
    return 0;
}
#else
void* ThreadFunctionStatic(void* _data)
{
    FThread* thread = static_cast<FThread*>(_data);
    thread->ThreadFunction();
    pthread_exit(nullptr);
    return nullptr;
}
#endif

ThreadID FThread::mainThreadID = FThread::CurrentThreadID();

FThread::FThread() :
    _handle(nullptr),
    _shouldRun(false)
{
}

FThread::~FThread()
{
    Stop();
}

bool FThread::Run()
{
    // Check if already running
    if (_handle)
        return false;
    
    _shouldRun = true;
    #ifdef _WIN32
    _handle = CreateThread(nullptr, 0, ThreadFunctionStatic, this, 0, nullptr);
    #else
    _handle = new pthread_t;
    pthread_attr_t _type;
    pthread_attr_init(&_type);
    pthread_attr_setdetachstate(&_type, PTHREAD_CREATE_JOINABLE);
    pthread_create((pthread_t*)_handle, &_type, ThreadFunctionStatic, this);
    #endif
    return _handle != nullptr;
}

void FThread::Stop()
{
    // Check if already stopped
    if (!_handle)
        return;
    
    _shouldRun = false;
    #ifdef _WIN32
    WaitForSingleObject((HANDLE)_handle, INFINITE);
    CloseHandle((HANDLE)_handle);
    #else
    pthread_t* t = (pthread_t*)_handle;
    if (t)
        pthread_join(*t, 0);
    delete t;
    #endif
    _handle = nullptr;
}

void FThread::SetPriority(int priority)
{
    #ifdef _WIN32
    if (_handle)
        SetThreadPriority((HANDLE)_handle, priority);
    #endif
    #if defined(__linux__) && !defined(ANDROID)
    pthread_t* t = (pthread_t*)_handle;
    if (t)
        pthread_setschedprio(*t, priority);
    #endif
}

void FThread::Sleep(unsigned mSec)
{
    #ifdef _WIN32
    ::Sleep(mSec);
    #else
    usleep(mSec * 1000);
    #endif
}

void FThread::SetMainThread()
{
    mainThreadID = CurrentThreadID();
}

ThreadID FThread::CurrentThreadID()
{
    #ifdef _WIN32
    return GetCurrentThreadId();
    #else
    return pthread_self();
    #endif
}

bool FThread::IsMainThread()
{
    return CurrentThreadID() == mainThreadID;
}

}
