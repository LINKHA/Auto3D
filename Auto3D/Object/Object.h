#pragma once

#include "../Base/StringHash.h"
#include "Event.h"

namespace Auto3D
{

class ObjectFactory;
template <class T> class ObjectFactoryImpl;

/// Base class for objects with type identification and possibility to create through a factory.
class AUTO_API Object : public RefCounted
{
public:
    /// Return hash of the type name.
    virtual StringHash Type() const = 0;
    /// Return type name.
    virtual const String& TypeName() const = 0;

    /// Subscribe to an event.
    void SubscribeToEvent(Event& event, EventHandler* handler);
    /// Unsubscribe from an event.
    void UnsubscribeFromEvent(Event& event);
    /// Send an event.
    void SendEvent(Event& event);
    
    /// Subscribe to an event, template version.
    template <class T, class U> void SubscribeToEvent(U& event, void (T::*handlerFunction)(U&))
    {
        SubscribeToEvent(event, new EventHandlerImpl<T, U>(this, handlerFunction)); 
    }

    /// Return whether is subscribed to an event.
    bool IsSubscribedToEvent(const Event& event) const;
    
    /// Register an object as a subsystem that can be accessed globally. Note that the subsystems container does not own the objects.
    static void RegisterSubsystem(Object* subsystem);
    /// Remove a subsystem by object pointer.
    static void RemoveSubsystem(Object* subsystem);
    /// Remove a subsystem by type.
    static void RemoveSubsystem(StringHash type);
    /// Return a subsystem by type, or null if not registered.
    static Object* Subsystem(StringHash type);
    /// Register an object factory.
    static void RegisterFactory(ObjectFactory* factory);
    /// Create and return an object through a factory. The caller is assumed to take ownership of the object. Return null if no factory registered. 
    static Object* Create(StringHash type);
    /// Return a type name from hash, or empty if not known. Requires a registered object factory.
    static const String& TypeNameFromType(StringHash type);
    /// Return a subsystem, template version.
    template <class T> static T* Subsystem() { return static_cast<T*>(Subsystem(T::TypeStatic())); }
    /// Register an object factory, template version.
    template <class T> static void RegisterFactory() { RegisterFactory(new ObjectFactoryImpl<T>()); }
    /// Create and return an object through a factory, template version.
    template <class T> static T* Create() { return static_cast<T*>(Create(T::TypeStatic())); }
    
private:
    /// Registered subsystems.
    static HashMap<StringHash, Object*> subsystems;
    /// Registered object factories.
    static HashMap<StringHash, AutoPtr<ObjectFactory> > factories;
};

/// Base class for object factories.
class AUTO_API ObjectFactory
{
public:
    /// Destruct.
    virtual ~ObjectFactory();
    
    /// Create and return an object.
    virtual Object* Create() = 0;

    /// Return type name hash of the objects created by this factory.
    StringHash Type() const { return type; }
    /// Return type name of the objects created by this factory.
    const String& TypeName() const { return typeName; }

protected:
    /// %Object type name hash.
    StringHash type;
    /// %Object type name.
    String typeName;
};

/// Template implementation of the object factory.
template <class T> class ObjectFactoryImpl : public ObjectFactory
{
public:
    /// Construct.
    ObjectFactoryImpl()
    {
        type = T::TypeStatic();
        typeName = T::TypeNameStatic();
    }

    /// Create and return an object of the specific type.
    Object* Create() override { return new T(); }
};

}

#define REGISTER_OBJECT_CLASS(_This,_Base) \
public: \
	using This = _This;\
	using Super = _Base;\
	Auto3D::StringHash Type() const override { return TypeStatic(); } \
    const Auto3D::String& TypeName() const override { return TypeNameStatic(); } \
    static Auto3D::StringHash TypeStatic() { static const Auto3D::StringHash type(#_This); return type; } \
    static const Auto3D::String& TypeNameStatic() { static const Auto3D::String type(#_This); return type; } \
private: \
    static const Auto3D::StringHash typeStatic; \
    static const Auto3D::String typeNameStatic; \
public: \
       

