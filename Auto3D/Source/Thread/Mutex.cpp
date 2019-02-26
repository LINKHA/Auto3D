#include "Mutex.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include "../Debug/DebugNew.h"

namespace Auto3D
{

#ifdef _WIN32
Mutex::Mutex() :
    _handle(new CRITICAL_SECTION)
{
    InitializeCriticalSection((CRITICAL_SECTION*)_handle);
}

Mutex::~Mutex()
{
    CRITICAL_SECTION* cs = (CRITICAL_SECTION*)_handle;
    DeleteCriticalSection(cs);
    delete cs;
    _handle = nullptr;
}

void Mutex::Acquire()
{
    EnterCriticalSection((CRITICAL_SECTION*)_handle);
}

void Mutex::Release()
{
    LeaveCriticalSection((CRITICAL_SECTION*)_handle);
}
#else
Mutex::Mutex() :
    _handle(new pthread_mutex_t)
{
    pthread_mutex_t* m = (pthread_mutex_t*)_handle;
    pthread_mutexattr_t _attr;
    pthread_mutexattr_init(&_attr);
    pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(m, &_attr);
}

Mutex::~Mutex()
{
    pthread_mutex_t* m = (pthread_mutex_t*)_handle;
    pthread_mutex_destroy(m);
    delete m;
    _handle = nullptr;
}

void Mutex::Acquire()
{
    pthread_mutex_lock((pthread_mutex_t*)_handle);
}

void Mutex::Release()
{
    pthread_mutex_unlock((pthread_mutex_t*)_handle);
}
#endif

MutexLock::MutexLock(Mutex& mutex) :
    _mutex(mutex)
{
    _mutex.Acquire();
}

MutexLock::~MutexLock()
{
    _mutex.Release();
}

}
