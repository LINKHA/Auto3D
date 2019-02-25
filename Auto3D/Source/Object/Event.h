#pragma once

#include "../Base/AutoPtr.h"
#include "../Base/Ptr.h"
#include "../IO/JSONValue.h"

namespace Auto3D
{

class Event;

/// Internal helper class for invoking event handler functions.
class AUTO_API EventHandler
{
public:
    /// Construct with receiver object pointer.
    EventHandler(RefCounted* receiver);
    /// Destruct.
    virtual ~EventHandler();

    /// Invoke the handler function. Implemented by subclasses.
    virtual void Invoke(Event& event) = 0;

    /// Return the receiver object.
    RefCounted* Receiver() const { return receiver.Get(); }

protected:
    /// Receiver object.
    WeakPtr<RefCounted> receiver;
};

/// Template implementation of the event handler invoke helper, stores a function pointer of specific class.
template <class _Ty, class U> class EventHandlerImpl : public EventHandler
{
public:
    typedef void (_Ty::*HandlerFunctionPtr)(U&);

    /// Construct with receiver and function pointers.
    EventHandlerImpl(RefCounted* receiver_, HandlerFunctionPtr function_) :
        EventHandler(receiver_),
        function(function_)
    {
        assert(function);
    }

    /// Invoke the handler function.
    void Invoke(Event& event) override
    {
        _Ty* typedReceiver = static_cast<_Ty*>(receiver.Get());
        U& typedEvent = static_cast<U&>(event);
        (typedReceiver->*function)(typedEvent);
    }

private:
    /// Pointer to the event handler function.
    HandlerFunctionPtr function;
};

/// Notification and data passing mechanism, to which objects can subscribe by specifying a handler function. Subclass to include event-specific data.
class AUTO_API Event
{
public:
    /// Construct.
    Event();
    /// Destruct.
    virtual ~Event();
    
    /// Send the event.
    void Send(RefCounted* sender);
    /// Subscribe to the event. The event takes ownership of the handler data. If there is already handler data for the same receiver, it is overwritten.
    void Subscribe(EventHandler* handler);
    /// Unsubscribe from the event.
    void Unsubscribe(RefCounted* receiver);

    /// Return whether has at least one valid receiver.
    bool HasReceivers() const;
    /// Return whether has a specific receiver.
    bool HasReceiver(const RefCounted* receiver) const;
    /// Return current sender.
    RefCounted* Sender() const { return currentSender; }
    
private:
    /// Prevent copy construction.
    Event(const Event& rhs);
    /// Prevent assignment.
    Event& operator = (const Event& rhs);
    
    /// Event handlers.
    Vector<AutoPtr<EventHandler> > handlers;
    /// Current sender.
    WeakPtr<RefCounted> currentSender;
};

}
