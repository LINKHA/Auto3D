#include "EventManager.h"

#include "Debug/DebugNew.h"

namespace Auto3D {

IMPLEMENT_SINGLETON(GEventManager)

GEventManager::GEventManager() 
{}

GEventManager::~GEventManager()
{}

void GEventManager::SubscribeToEvent(FEvent& event, FEventHandler* handler)
{
	event.Subscribe(handler);
}

void GEventManager::UnsubscribeFromEvent(FRefCounted* sender, FEvent& event)
{
	event.Unsubscribe(sender);
}

void GEventManager::SendEvent(FRefCounted* sender, FEvent& event)
{
	event.Send(sender);
}

bool GEventManager::IsSubscribedToEvent(FRefCounted* sender, const FEvent& event) const
{
	return event.HasReceiver(sender);
}

}