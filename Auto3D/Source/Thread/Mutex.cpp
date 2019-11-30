#include "Mutex.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include "Debug/DebugNew.h"

namespace Auto3D
{

#ifdef _WIN32
FMutex::FMutex() :
    _handle(new CRITICAL_SECTION)
{
    InitializeCriticalSection((CRITICAL_SECTION*)_handle);
}

FMutex::~FMutex()
{
    CRITICAL_SECTION* cs = (CRITICAL_SECTION*)_handle;
    DeleteCriticalSection(cs);
    delete cs;
    _handle = nullptr;
}

void FMutex::Acquire()
{
    EnterCriticalSection((CRITICAL_SECTION*)_handle);
}

void FMutex::Release()
{
    LeaveCriticalSection((CRITICAL_SECTION*)_handle);
}
#else
FMutex::FMutex() :
    _handle(new pthread_mutex_t)
{
    pthread_mutex_t* m = (pthread_mutex_t*)_handle;
    pthread_mutexattr_t _attr;
    pthread_mutexattr_init(&_attr);
    pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(m, &_attr);
}

FMutex::~FMutex()
{
    pthread_mutex_t* m = (pthread_mutex_t*)_handle;
    pthread_mutex_destroy(m);
    delete m;
    _handle = nullptr;
}

void FMutex::Acquire()
{
    pthread_mutex_lock((pthread_mutex_t*)_handle);
}

void FMutex::Release()
{
    pthread_mutex_unlock((pthread_mutex_t*)_handle);
}
#endif

FMutexLock::FMutexLock(FMutex& mutex) :
    _mutex(mutex)
{
    _mutex.Acquire();
}

FMutexLock::~FMutexLock()
{
    _mutex.Release();
}

}
