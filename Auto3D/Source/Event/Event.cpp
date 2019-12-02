#include "Debug/Log.h"
#include "Thread/Thread.h"
#include "Event.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

FEventHandler::FEventHandler(FRefCounted* receiver_) :
    _receiver(receiver_)
{
}

FEventHandler::~FEventHandler()
{
}

FEvent::FEvent()
{
}

FEvent::~FEvent()
{
}

void FEvent::Send(FRefCounted* sender)
{
    if (!FThread::IsMainThread())
    {
        ErrorString("Attempted to send an event from outside the main thread");
        return;
    }

    // Retain a weak pointer to the sender on the stack for safety, in case it is destroyed
    // as a result of _event handling, in which case the current _event may also be destroyed
    TWeakPtr<FRefCounted> safeCurrentSender = sender;
    _currentSender = sender;
    
    for (auto it = _handlers.Begin(); it != _handlers.End();)
    {
        FEventHandler* handler = *it;
        bool remove = true;
        
        if (handler)
        {
            FRefCounted* receiver = handler->Receiver();
            if (receiver)
            {
                remove = false;
                handler->Invoke(*this);
                // If the sender has been destroyed, abort processing immediately
                if (safeCurrentSender.IsExpired())
                    return;
            }
        }
        
        if (remove)
            it = _handlers.Erase(it);
        else
            ++it;
    }
    
    _currentSender.Reset();
}

void FEvent::Subscribe(FEventHandler* handler)
{
    if (!handler)
        return;
    
    // Check if the same receiver already exists; in that case replace the handler data
    for (auto it = _handlers.Begin(); it != _handlers.End(); ++it)
    {
        FEventHandler* existing = *it;
        if (existing && existing->Receiver() == handler->Receiver())
        {
            *it = handler;
            return;
        }
    }
    
    _handlers.Push(handler);
}

void FEvent::Unsubscribe(FRefCounted* receiver)
{
    for (auto it = _handlers.Begin(); it != _handlers.End(); ++it)
    {
        FEventHandler* handler = *it;
        if (handler && handler->Receiver() == receiver)
        {
            // If _event sending is going on, only clear the pointer but do not remove the element from the handler vector
            // to not confuse the _event sending iteration; the element will eventually be cleared by the next SendEvent().
            if (_currentSender)
                *it = nullptr;
            else
                _handlers.Erase(it);
            return;
        }
    }
}

bool FEvent::HasReceivers() const
{
    for (auto it = _handlers.Begin(); it != _handlers.End(); ++it)
    {
        FEventHandler* handler = *it;
        if (handler && handler->Receiver())
            return true;
    }
    
    return false;
}

bool FEvent::HasReceiver(const FRefCounted* receiver) const
{
    for (auto it = _handlers.Begin(); it != _handlers.End(); ++it)
    {
        FEventHandler* handler = *it;
        if (handler && handler->Receiver() == receiver)
            return true;
    }
    
    return false;
}

}
