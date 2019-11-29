#pragma once

#include "Container/StringHash.h"
#include "Event.h"
#include "Math/AutoMath.h"

namespace Auto3D
{

class IObjectFactory;
template <typename _Ty> class TObjectFactoryImpl;


/// Type info.
class AUTO_API FTypeInfo
{
public:
	/// Construct.
	FTypeInfo(const char* typeName, const FTypeInfo* baseTypeInfo);
	/// Destruct.
	~FTypeInfo();

	/// Check current type is type of specified type.
	bool IsTypeOf(FStringHash type) const;
	/// Check current type is type of specified type.
	bool IsTypeOf(const FTypeInfo* typeInfo) const;
	/// Check current type is type of specified class type.
	template<typename _Ty> bool IsTypeOf() const { return IsTypeOf(_Ty::GetTypeInfoStatic()); }

	/// Return type.
	FStringHash GetType() const { return _type; }
	/// Return type name.
	const FString& GetTypeName() const { return _typeName; }
	/// Return base type info.
	const FTypeInfo* GetBaseTypeInfo() const { return _baseTypeInfo; }

private:
	/// Type.
	FStringHash _type;
	/// Type name.
	FString _typeName;
	/// Base class type info.
	const FTypeInfo* _baseTypeInfo;
};

#define REGISTER_OBJECT_CLASS(_This,_Base) \
public: \
	_This& operator=(_This&&) = delete;   \
    _This& operator=(const _This&)= delete;  \
	using This = _This;\
	using Super = _Base;\
	virtual Auto3D::FStringHash GetType() const override { return GetTypeStatic(); } \
	virtual const Auto3D::FString& GetTypeName() const override { return GetTypeNameStatic(); } \
    virtual const Auto3D::FTypeInfo* GetTypeInfo() const override { return GetTypeInfoStatic(); } \
	static Auto3D::FStringHash GetTypeStatic() { static const Auto3D::FStringHash type(#_This); return type; } \
    static const Auto3D::FString& GetTypeNameStatic() { static const Auto3D::FString type(#_This); return type; } \
	static const Auto3D::FTypeInfo* GetTypeInfoStatic() { static const Auto3D::FTypeInfo typeInfoStatic(#_This, _Base::GetTypeInfoStatic()); return &typeInfoStatic; } \
private: \
    static const Auto3D::FStringHash typeStatic; \
    static const Auto3D::FString typeNameStatic; \
public: \


/// Base class for objects with type identification and possibility to create through a factory.
class AUTO_API AObject : public FRefCounted
{
public:
	/// Structure
	AObject() = default;
	/// Destructor
	virtual ~AObject() = default;

    /// Return hash of the type name.
    virtual FStringHash GetType() const = 0;
    /// Return type name.
    virtual const FString& GetTypeName() const = 0;
	/// Return type info.
	virtual const FTypeInfo* GetTypeInfo() const = 0;

	/// Return type info static.
	static const FTypeInfo* GetTypeInfoStatic() { return nullptr; }
	/// Check current instance is type of specified type.
	bool IsInstanceOf(FStringHash type) const;
	/// Check current instance is type of specified type.
	bool IsInstanceOf(const FTypeInfo* typeInfo) const;
	/// Check current instance is type of specified class.
	template<typename _Ty> bool IsInstanceOf() const { return IsInstanceOf(_Ty::GetTypeInfoStatic()); }
	/// Cast the object to specified most derived class.
	template<typename _Ty> _Ty* Cast() { return IsInstanceOf<_Ty>() ? static_cast<_Ty*>(this) : nullptr; }
	/// Cast the object to specified most derived class.
	template<typename _Ty> const _Ty* Cast() const { return IsInstanceOf<_Ty>() ? static_cast<const _Ty*>(this) : nullptr; }

	/// Subscribe to an _event.
    void SubscribeToEvent(FEvent& event, FEventHandler* handler);
    /// Unsubscribe from an _event.
    void UnsubscribeFromEvent(FEvent& event);
    /// Send an _event.
    void SendEvent(FEvent& event);
    
    /// Subscribe to an _event, template version.
    template <typename _Ty, typename _Event> void SubscribeToEvent(_Event& event, void (_Ty::*handlerFunction)(_Event&))
    {
        SubscribeToEvent(event, new TEventHandlerImpl<_Ty, _Event>(this, handlerFunction));
    }

    /// Return whether is subscribed to an event.
    bool IsSubscribedToEvent(const FEvent& event) const;
    
    /// Register an object as a module that can be accessed globally. Note that the modules container does not own the objects.
    static void RegisterObjectModule(AObject* module);
    /// Remove a module by object pointer.
    static void RemoveObjectModule(AObject* module);
    /// Remove a module by type.
    static void RemoveObjectModule(FStringHash type);
    /// Return a subsystem by type, or null if not registered.
    static AObject* ObjectModule(FStringHash type);
    /// Register an object factory.
    static void RegisterFactory(IObjectFactory* factory);
    /// Create and return an object through a factory. The caller is assumed to take ownership of the object. Return null if no factory registered. 
    static AObject* Create(FStringHash type);
    /// Return a type name from hash, or empty if not known. Requires a registered object factory.
    static const FString& TypeNameFromType(FStringHash type);
    /// Return a subsystem, template version.
    template <typename _Ty> static _Ty* ObjectModule() { return static_cast<_Ty*>(ObjectModule(_Ty::GetTypeStatic())); }
    /// Register an object factory, template version.
    template <typename _Ty> static void RegisterFactory() { RegisterFactory(new TObjectFactoryImpl<_Ty>()); }
    /// Create and return an object through a factory, template version.
    template <typename _Ty> static _Ty* Create() { return static_cast<_Ty*>(Create(_Ty::GetTypeStatic())); }
    
private:
    /// Registered modules.
    static THashMap<FStringHash, AObject*> _objectModules;
    /// Registered object factories.
    static THashMap<FStringHash, TAutoPtr<IObjectFactory> > _factories;
};

/// Base class for object factories.
class AUTO_API IObjectFactory
{
public:
    /// Destruct.
    virtual ~IObjectFactory();
    
    /// Create and return an object.
    virtual AObject* Create() = 0;

    /// Return type name hash of the objects created by this factory.
    FStringHash GetType() const { return _type; }
    /// Return type name of the objects created by this factory.
    const FString& GetTypeName() const { return _typeName; }

protected:
    /// %AObject type name hash.
    FStringHash _type;
    /// %AObject type name.
    FString _typeName;
};

/// Template implementation of the object factory.
template <typename _Ty> class TObjectFactoryImpl : public IObjectFactory
{
public:
    /// Construct.
    TObjectFactoryImpl()
    {
        _type = _Ty::GetTypeStatic();
        _typeName = _Ty::GetTypeNameStatic();
    }

    /// Create and return an object of the specific type.
    AObject* Create() override { return new _Ty(); }
};


}
