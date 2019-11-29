#pragma once

#include "Container/AutoPtr.h"
#include "Container/Ptr.h"
#include "IO/JSONValue.h"

namespace Auto3D
{

class FEvent;

/// Internal helper class for invoking event handler functions.
class AUTO_API FEventHandler
{
public:
    /// Construct with receiver object pointer.
    FEventHandler(FRefCounted* receiver);
    /// Destruct.
    virtual ~FEventHandler();

    /// Invoke the handler function. Implemented by subclasses.
    virtual void Invoke(FEvent& event) = 0;

    /// Return the receiver object.
    FRefCounted* Receiver() const { return _receiver.Get(); }

protected:
    /// Receiver object.
    TWeakPtr<FRefCounted> _receiver;
};

/// Template implementation of the event handler invoke helper, stores a function pointer of specific class.
template <typename _Ty, typename _Event> class TEventHandlerImpl : public FEventHandler
{
public:
    typedef void (_Ty::*HandlerFunctionPtr)(_Event&);

    /// Construct with receiver and function pointers.
    TEventHandlerImpl(FRefCounted* receiver, HandlerFunctionPtr function) :
        FEventHandler(receiver),
        _function(function)
    {
        assert(_function);
    }

    /// Invoke the handler function.
    void Invoke(FEvent& event) override
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
class AUTO_API FEvent
{
public:
    /// Construct.
    FEvent();
    /// Destruct.
    virtual ~FEvent();
	/// Prevent copy construction.
	FEvent(const FEvent& rhs) = delete;
	/// Prevent assignment.
	FEvent& operator = (const FEvent& rhs) = delete;


    /// Send the _event.
    void Send(FRefCounted* sender);
    /// Subscribe to the _event. The _event takes ownership of the handler data. If there is already handler data for the same receiver, it is overwritten.
    void Subscribe(FEventHandler* handler);
    /// Unsubscribe from the _event.
    void Unsubscribe(FRefCounted* receiver);

    /// Return whether has at least one _valid receiver.
    bool HasReceivers() const;
    /// Return whether has a specific receiver.
    bool HasReceiver(const FRefCounted* receiver) const;
    /// Return current sender.
    FRefCounted* Sender() const { return _currentSender; }

private:
    /// FEvent handlers.
    TVector<TAutoPtr<FEventHandler> > _handlers;
    /// Current sender.
    TWeakPtr<FRefCounted> _currentSender;
};

}
