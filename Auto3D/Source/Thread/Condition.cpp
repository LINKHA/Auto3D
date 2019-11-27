#include "Condition.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include "Debug/DebugNew.h"

namespace Auto3D
{

#ifdef _WIN32
Condition::Condition() :
    _event(nullptr)
{
    _event = CreateEvent(0, FALSE, FALSE, 0);
}

Condition::~Condition()
{
    CloseHandle((HANDLE)_event);
    _event = nullptr;
}

void Condition::Set()
{
    SetEvent((HANDLE)_event);
}

void Condition::Wait()
{
    WaitForSingleObject((HANDLE)_event, INFINITE);
}
#else
Condition::Condition() :
	_mutex(new pthread_mutex_t),
    _event(new pthread_cond_t)
{
    pthread_mutex_init((pthread_mutex_t*)_mutex, 0);
    pthread_cond_init((pthread_cond_t*)_event, 0);
}

Condition::~Condition()
{
    pthread_cond_t* c = (pthread_cond_t*)_event;
    pthread_mutex_t* m = (pthread_mutex_t*)_mutex;

    pthread_cond_destroy(c);
    pthread_mutex_destroy(m);
    delete c;
    delete m;
    _event = nullptr;
	_mutex = nullptr;
}

void Condition::Set()
{
    pthread_cond_signal((pthread_cond_t*)_event);
}

void Condition::Wait()
{
    pthread_cond_t* c = (pthread_cond_t*)_event;
    pthread_mutex_t* m = (pthread_mutex_t*)_mutex;

    pthread_mutex_lock(m);
    pthread_cond_wait(c, m);
    pthread_mutex_unlock(m);
}
#endif

}
