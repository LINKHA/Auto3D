#pragma once

#include "Container/AutoPtr.h"
#include "Container/Ptr.h"
#include "IO/JSONValue.h"

namespace Auto3D
{

class Event;

/// Internal helper class for invoking event handler functions.
class AUTO_API EventHandler
{
public:
    /// Construct with receiver object pointer.
    EventHandler(ARefCounted* receiver);
    /// Destruct.
    virtual ~EventHandler();

    /// Invoke the handler function. Implemented by subclasses.
    virtual void Invoke(Event& event) = 0;

    /// Return the receiver object.
    ARefCounted* Receiver() const { return _receiver.Get(); }

protected:
    /// Receiver object.
    WeakPtr<ARefCounted> _receiver;
};

/// Template implementation of the event handler invoke helper, stores a function pointer of specific class.
template <typename _Ty, typename _Event> class EventHandlerImpl : public EventHandler
{
public:
    typedef void (_Ty::*HandlerFunctionPtr)(_Event&);

    /// Construct with receiver and function pointers.
    EventHandlerImpl(ARefCounted* receiver, HandlerFunctionPtr function) :
        EventHandler(receiver),
        _function(function)
    {
        assert(_function);
    }

    /// Invoke the handler function.
    void Invoke(Event& event) override
    {
        _Ty* typedReceiver = static_cast<_Ty*>(_receiver.Get());
		_Event& typedEvent = static_cast<_Event&>(event);
        (typedReceiver->*_function)(typedEvent);
    }

private:
    /// Pointer to the event handler function.
    HandlerFunctionPtr _function;
};

/// Notification and data passing mechanism, to which objects can subscribe by specifying a handler function. Subclass to include _event-specific data.
class AUTO_API Event
{
public:
    /// Construct.
    Event();
    /// Destruct.
    virtual ~Event();
	/// Prevent copy construction.
	Event(const Event& rhs) = delete;
	/// Prevent assignment.
	Event& operator = (const Event& rhs) = delete;


    /// Send the _event.
    void Send(ARefCounted* sender);
    /// Subscribe to the _event. The _event takes ownership of the handler data. If there is already handler data for the same receiver, it is overwritten.
    void Subscribe(EventHandler* handler);
    /// Unsubscribe from the _event.
    void Unsubscribe(ARefCounted* receiver);

    /// Return whether has at least one _valid receiver.
    bool HasReceivers() const;
    /// Return whether has a specific receiver.
    bool HasReceiver(const ARefCounted* receiver) const;
    /// Return current sender.
    ARefCounted* Sender() const { return _currentSender; }

private:
    /// Event handlers.
    Vector<AutoPtr<EventHandler> > _handlers;
    /// Current sender.
    WeakPtr<ARefCounted> _currentSender;
};

}
