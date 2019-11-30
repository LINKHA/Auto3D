#include "ThreadLocalValue.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Debug/DebugNew.h"

namespace Auto3D
{

FThreadLocalValue::FThreadLocalValue()
{
    #ifdef _WIN32
    _key = TlsAlloc();
    _valid = _key != TLS_OUT_OF_INDEXES;
    #else
    _valid = pthread_key_create(&_key, 0) == 0;
    #endif
}

FThreadLocalValue::~FThreadLocalValue()
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

void FThreadLocalValue::SetValue(void* value)
{
    if (_valid)
    {
        #ifdef _WIN32
        TlsSetValue(_key, value);
        #else
        pthread_setspecific(_key, _value);
        #endif
    }
}

void* FThreadLocalValue::Value() const
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
