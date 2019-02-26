#include "ThreadLocalValue.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "../Debug/DebugNew.h"

namespace Auto3D
{

ThreadLocalValue::ThreadLocalValue()
{
    #ifdef _WIN32
    _key = TlsAlloc();
    _valid = _key != TLS_OUT_OF_INDEXES;
    #else
    _valid = pthread_key_create(&_key, 0) == 0;
    #endif
}

ThreadLocalValue::~ThreadLocalValue()
{
    if (_valid)
    {
        #ifdef _WIN32
        TlsFree(_key);
        #else
        pthread_key_delete(_key);
        #endif
    }
}

void ThreadLocalValue::SetValue(void* value)
{
    if (_valid)
    {
        #ifdef _WIN32
        TlsSetValue(_key, value);
        #else
        pthread_setspecific(_key, value);
        #endif
    }
}

void* ThreadLocalValue::Value() const
{
    if (_valid)
    {
        #ifdef _WIN32
        return TlsGetValue(_key);
        #else
        return pthread_getspecific(_key);
        #endif
    }
    else
        return nullptr;
}

}
