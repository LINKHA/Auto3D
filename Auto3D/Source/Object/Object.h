#pragma once

#include "../Base/StringHash.h"
#include "Event.h"
#include "../AutoCommon.h"

namespace Auto3D
{

class ObjectFactory;
template <class _Ty> class ObjectFactoryImpl;

/// Base class for objects with type identification and possibility to create through a factory.
class AUTO_API Object : public RefCounted
{
public:
    /// Return hash of the type name.
    virtual StringHash Type() const = 0;
    /// Return type name.
    virtual const String& TypeName() const = 0;

    /// Subscribe to an _event.
    void SubscribeToEvent(Event& event, EventHandler* handler);
    /// Unsubscribe from an _event.
    void UnsubscribeFromEvent(Event& event);
    /// Send an _event.
    void SendEvent(Event& event);
    
    /// Subscribe to an _event, template version.
    template <class _Ty, class U> void SubscribeToEvent(U& event, void (_Ty::*handlerFunction)(U&))
    {
        SubscribeToEvent(event, new EventHandlerImpl<_Ty, U>(this, handlerFunction)); 
    }

    /// Return whether is subscribed to an _event.
    bool IsSubscribedToEvent(const Event& event) const;
    
    /// Register an object as a subsystem that can be accessed globally. Note that the subsystems container does not own the objects.
    static void RegisterSubsystem(Object* subsystem);
    /// Remove a subsystem by object pointer.
    static void RemoveSubsystem(Object* subsystem);
    /// Remove a subsystem by type.
    static void RemoveSubsystem(StringHash type);
    /// Return a subsystem by type, or null if not registered.
    static Object* GetSubsystem(StringHash type);
    /// Register an object factory.
    static void RegisterFactory(ObjectFactory* factory);
    /// Create and return an object through a factory. The caller is assumed to take ownership of the object. Return null if no factory registered. 
    static Object* Create(StringHash type);
    /// Return a type name from hash, or empty if not known. Requires a registered object factory.
    static const String& TypeNameFromType(StringHash type);
    /// Return a subsystem, template version.
    template <class _Ty> static _Ty* Subsystem() { return static_cast<_Ty*>(GetSubsystem(_Ty::TypeStatic())); }
    /// Register an object factory, template version.
    template <class _Ty> static void RegisterFactory() { RegisterFactory(new ObjectFactoryImpl<_Ty>()); }
    /// Create and return an object through a factory, template version.
    template <class _Ty> static _Ty* Create() { return static_cast<_Ty*>(Create(_Ty::TypeStatic())); }
    
private:
    /// Registered subsystems.
    static HashMap<StringHash, Object*> _subsystems;
    /// Registered object factories.
    static HashMap<StringHash, AutoPtr<ObjectFactory> > _factories;
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
    StringHash Type() const { return _type; }
    /// Return type name of the objects created by this factory.
    const String& TypeName() const { return _typeName; }

protected:
    /// %Object type name hash.
    StringHash _type;
    /// %Object type name.
    String _typeName;
};

/// Template implementation of the object factory.
template <class _Ty> class ObjectFactoryImpl : public ObjectFactory
{
public:
    /// Construct.
    ObjectFactoryImpl()
    {
        _type = _Ty::TypeStatic();
        _typeName = _Ty::TypeNameStatic();
    }

    /// Create and return an object of the specific type.
    Object* Create() override { return new _Ty(); }
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
       

