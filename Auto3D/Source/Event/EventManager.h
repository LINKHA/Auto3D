#include "AutoConfig.h"
#include "Container/Singleton.h"
#include "Event/Event.h"

namespace Auto3D
{
	/// Event manager , use must 
class AUTO_API GEventManager
{
	REGISTER_SINGLETON(GEventManager)
protected:
	/// Structure
	GEventManager();
	/// Destructor
	~GEventManager();
public:
	/// Subscribe to an _event.
	void SubscribeToEvent(FEvent& event, IEventHandler* handler);
	/// Unsubscribe from an event.
	void UnsubscribeFromEvent(FRefCounted* sender, FEvent& event);
	/// Send an event from sender.
	void SendEvent(FRefCounted* sender, FEvent& event);

	/// Subscribe to an _event, template version.
	template <typename _Ty, typename _Event> void SubscribeToEvent(_Event& event, void (_Ty::*handlerFunction)(_Event&))
	{
		SubscribeToEvent(event, new TEventHandlerImpl<_Ty, _Event>(this, handlerFunction));
	}

	/// Return whether is subscribed to an event.
	bool IsSubscribedToEvent(FRefCounted* sender, const FEvent& event) const;

};

}