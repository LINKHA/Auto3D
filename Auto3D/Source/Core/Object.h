#pragma once
#include "AutoConfig.h"
#include "Adapter/AutoRttr.h"
#include "Adapter/Ptr.h"
#include "Container/HashMap.h"
#include "Container/Allocator.h"
namespace Auto3D
{


#define DECLARE_RTTR_BASE_CLASS(_This) \
	/*The base Object does not need to specify a parent class*/\
	RTTR_ENABLE() \
	/*Reflected private tag*/\
	RTTR_REGISTRATION_FRIEND 


#define DECLARE_RTTR_CLASS(_This,_Base) \
	/*The base Object does not need to specify a parent class*/\
	RTTR_ENABLE(_Base) \
	/*Reflected private tag*/\
	RTTR_REGISTRATION_FRIEND 


#define DECLARE_A_CLASS(_This,_Base) \
public: \
	_This& operator=(_This&&) = delete;   \
	_This& operator=(const _This&)= delete;  \
	using This = _This;\
	using Super = _Base;\
	virtual const Auto3D::FString& GetTypeName() const override { return GetTypeNameStatic(); } \
	static const Auto3D::FString& GetTypeNameStatic() { static const Auto3D::FString type(#_This); return type; } \
	DECLARE_RTTR_CLASS(_This, _Base) \
private:\
	SPtr<_This> SPtrThis() {return std::dynamic_pointer_cast<_This>(shared_from_this());}\
	WPtr<_This> WPtrThis() { return std::dynamic_pointer_cast<_This>(shared_from_this()); }\
public:


#define REGISTER_A_CALSS_IMP(_Class) \
	OObject::RegisterFactory<_Class>();\
	using namespace rttr;\
	registration::class_<_Class>(#_Class)

#define DECLARE_O_CLASS(_This,_Base) \
public: \
	_This& operator=(_This&&) = delete;   \
	_This& operator=(const _This&)= delete;  \
	using This = _This;\
	using Super = _Base;\
	virtual const Auto3D::FString& GetTypeName() const override { return GetTypeNameStatic(); } \
	static const Auto3D::FString& GetTypeNameStatic() { static const Auto3D::FString type(#_This); return type; } \
private:\
	SPtr<_This> SPtrThis() {return std::dynamic_pointer_cast<_This>(shared_from_this());}\
	WPtr<_This> WPtrThis() { return std::dynamic_pointer_cast<_This>(shared_from_this()); }\
public:

#define REGISTER_O_CALSS_IMP(_Class) \
	OObject::RegisterFactory<_Class>();\
	using namespace rttr;\
	registration::class_<_Class>(#_Class)
	

#define REGISTER_REFLECTION_FUNCATION rttr_auto_register_reflection_function_

class FObjectFactory;
template <typename _Ty> class TObjectFactoryImpl;

/// Base class for objects.
class AUTO_API OObject : public IEnablePtrThis<OObject>
{
	DECLARE_RTTR_BASE_CLASS(OObject)
public:
	/// Structure
	OObject() = default;
	/// Destructor
	virtual ~OObject() = default;
	virtual const Auto3D::FString& GetTypeName() const { return GetTypeNameStatic(); } 
	static const Auto3D::FString& GetTypeNameStatic() { static const Auto3D::FString type("OObject"); return type; } 

	/// Register an object factory.
	static void RegisterFactory(SPtr<FObjectFactory> factory);
	/// Register an object factory, template version.
	template <typename _Ty> static void RegisterFactory() { RegisterFactory(MakeShared<TObjectFactoryImpl<_Ty>>()); }
};

/// Create and return an object through a factory. The caller is assumed to take ownership of the object. Return null if no factory registered. 
OObject* NewObject(FString type);
/// Create and return an object through a factory, template version.
template <typename _Ty> static _Ty* NewObject() { return dynamic_cast<_Ty*>(NewObject(_Ty::GetTypeNameStatic())); }

/// Base class for object factories.
class AUTO_API FObjectFactory
{
public:
	/// Destruct.
	virtual ~FObjectFactory() {}

	/// Create and return an object.
	virtual OObject* Create() = 0;

	/// Return type name of the objects created by this factory.
	const FString& GetTypeName() const { return _typeName; }

protected:
	/// %AObject type name.
	FString _typeName;
};

/// Template implementation of the object factory.
template <typename _Ty> class TObjectFactoryImpl : public FObjectFactory
{
public:
	/// Construct.
	TObjectFactoryImpl()
	{
		_typeName = _Ty::GetTypeNameStatic();
	}

	/// Create and return an object of the specific type.
	OObject* Create() override { return new _Ty(); }
};

}
