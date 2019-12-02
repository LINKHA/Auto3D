#pragma once
#include "Container/AutoPtr.h"
#include "Container/Ptr.h"
#include "IO/Stream.h"

namespace Auto3D
{

class FJSONValue;
class ASerializable;
class FStream;

/// Supported attribute types.
namespace EAttributeType
{
	enum Type
	{
		BOOL = 0,
		BYTE,
		UNSIGNED,
		INT,
		INTVECTOR2,
		INTRECT,
		FLOAT,
		VECTOR2,
		VECTOR3,
		VECTOR4,
		QUATERNION,
		COLOR,
		RECT,
		BOUNDINGBOX,
		MATRIX2,
		MATRIX3,
		MATRIX3X4,
		MATRIX4,
		STRING,
		RESOURCEREF,
		RESOURCEREFLIST,
		OBJECTREF,
		JSONVALUE,
		Count
	};
};


/// Helper class for accessing serializable variables via getter and setter functions.
class AUTO_API IAttributeAccessor
{
public:
    /// Destruct.
    virtual ~IAttributeAccessor();
    
    /// Get the current value of the variable.
    virtual void Get(const ASerializable* instance, void* dest) = 0;
    /// Set new value for the variable.
    virtual void Set(ASerializable* instance, const void* source) = 0;
};

/// Description of an automatically serializable variable.
class AUTO_API FAttribute : public FRefCounted
{
public:
    /// Construct.
    FAttribute(const char* name, IAttributeAccessor* accessor, const char** enumNames = 0);
	/// Prevent copy construction.
	FAttribute(const FAttribute& rhs) = delete;
	/// Prevent assignment.
	FAttribute& operator = (const FAttribute& rhs) = delete;

    /// Deserialize from a binary stream.
    virtual void FromBinary(ASerializable* instance, FStream& source) = 0;
    /// Serialize to a binary stream.
    virtual void ToBinary(ASerializable* instance, FStream& dest) = 0;
    /// Deserialize from JSON.
    virtual void FromJSON(ASerializable* instance, const FJSONValue& source) = 0;
    /// Serialize to JSON.
    virtual void ToJSON(ASerializable* instance, FJSONValue& dest) = 0;
    /// Return type.
    virtual EAttributeType::Type Type() const = 0;
    /// Return whether is default value.
    virtual bool IsDefault(ASerializable* instance) = 0;
    
    /// Set from a value in memory.
    void FromValue(ASerializable* instance, const void* source);
    /// Copy to a value in memory.
    void ToValue(ASerializable* instance, void* dest);
    
    /// Return variable name.
    const FString& GetName() const { return _name; }
    /// Return zero-based enum names, or null if none.
    const char** EnumNames() const { return _enumNames; }
    /// Return type name.
    const FString& TypeName() const;
    /// Return byte _size of the attribute data, or 0 if it can be variable.
    size_t ByteSize() const;
    
    /// Skip binary data of an attribute.
    static void Skip(EAttributeType::Type type, FStream& source);
    /// Serialize attribute value to JSON.
    static void ToJSON(EAttributeType::Type type, FJSONValue& dest, const void* source);
    /// Deserialize attribute value from JSON.
    static void FromJSON(EAttributeType::Type type, void* dest, const FJSONValue& source);
    /// Return attribute type from type name.
    static EAttributeType::Type TypeFromName(const FString& name);
    /// Return attribute type from type name.
    static EAttributeType::Type TypeFromName(const char* name);
    
    /// Type names.
    static const FString typeNames[];
    /// FAttribute byte sizes.
    static const size_t byteSizes[];

protected:
    /// Variable name.
    FString _name;
    /// FAttribute accessor.
	TUniquePtr<IAttributeAccessor> _accessor;
    /// Enum names.
    const char** _enumNames;

};

/// Template implementation of an attribute description with specific type.
template <typename _Ty> class TAttributeImpl : public FAttribute
{
public:
    /// Construct.
    TAttributeImpl(const char* name, IAttributeAccessor* accessor, const _Ty& defaultValue, const char** enumNames = 0) :
        FAttribute(name, accessor, enumNames),
        _defaultValue(defaultValue)
    {
    }
    
    /// Deserialize from a binary stream.
    void FromBinary(ASerializable* instance, FStream& source) override
    {
        _Ty value = source.Read<_Ty>();
        _accessor->Set(instance, &value);
    }
    
    /// Serialize to a binary stream.
    void ToBinary(ASerializable* instance, FStream& dest) override
    {
        _Ty value;
        _accessor->Get(instance, &value);
        dest.Write<_Ty>(value);
    }
    
    /// Return whether is default value.
    bool IsDefault(ASerializable* instance) override { return Value(instance) == _defaultValue; }
    
    /// Deserialize from JSON.
    void FromJSON(ASerializable* instance, const FJSONValue& source) override
    {
        _Ty value;
        FAttribute::FromJSON(Type(), &value, source);
        _accessor->Set(instance, &value);
    }

    /// Serialize to JSON.
    void ToJSON(ASerializable* instance, FJSONValue& dest) override
    {
        _Ty value;
        _accessor->Get(instance, &value);
        FAttribute::ToJSON(Type(), dest, &value);
    }

    /// Return type.
    EAttributeType::Type Type() const override;
    
    /// Set new attribute value.
    void SetValue(ASerializable* instance, const _Ty& source) { _accessor->Set(instance, &source); }
    /// Copy current attribute value.
    void Value(ASerializable* instance, _Ty& dest) { _accessor->Get(instance, &dest); }
    
    /// Return current attribute value.
    _Ty Value(ASerializable* instance)
    {
        _Ty ret;
        _accessor->Get(instance, &ret);
        return ret;
    }
    
    /// Return default value.
    const _Ty& DefaultValue() const { return _defaultValue; }
    
private:
    /// Default value.
    _Ty _defaultValue;
};

/// Template implementation for accessing serializable variables.
template <typename _Ty, typename U> class TAttributeAccessorImpl : public IAttributeAccessor
{
public:
    typedef U (_Ty::*GetFunctionPtr)() const;
    typedef void (_Ty::*SetFunctionPtr)(U);

    /// Construct with function pointers.
    TAttributeAccessorImpl(GetFunctionPtr getPtr, SetFunctionPtr setPtr) :
        _get(getPtr),
        _set(setPtr)
    {
        assert(_get);
        assert(_set);
    }

    /// Get current value of the variable.
    void Get(const ASerializable* instance, void* dest) override
    {
        assert(instance);

        U& value = *(reinterpret_cast<U*>(dest));
        const _Ty* classInstance = static_cast<const _Ty*>(instance);
        value = (classInstance->*_get)();
    }

    /// Set new value for the variable.
    void Set(ASerializable* instance, const void* source) override
    {
        assert(instance);

        const U& value = *(reinterpret_cast<const U*>(source));
        _Ty* classInstance = static_cast<_Ty*>(instance);
        (classInstance->*_set)(value);
    }

private:
    /// Getter function pointer.
    GetFunctionPtr _get;
    /// Setter function pointer.
    SetFunctionPtr _set;
};

/// Template implementation for accessing serializable variables via functions that use references.
template <typename _Ty, typename U> class TRefAttributeAccessorImpl : public IAttributeAccessor
{
public:
    typedef const U& (_Ty::*GetFunctionPtr)() const;
    typedef void (_Ty::*SetFunctionPtr)(const U&);

    /// Set new value for the variable.
    TRefAttributeAccessorImpl(GetFunctionPtr getPtr, SetFunctionPtr setPtr) :
        _get(getPtr),
        _set(setPtr)
    {
        assert(_get);
        assert(_set);
    }

    /// Get current value of the variable.
    void Get(const ASerializable* instance, void* dest) override
    {
        assert(instance);

        U& value = *(reinterpret_cast<U*>(dest));
        const _Ty* classPtr = static_cast<const _Ty*>(instance);
        value = (classPtr->*_get)();
    }

    /// Set new value for the variable.
    void Set(ASerializable* instance, const void* source) override
    {
        assert(instance);

        const U& value = *(reinterpret_cast<const U*>(source));
        _Ty* classPtr = static_cast<_Ty*>(instance);
        (classPtr->*_set)(value);
    }

private:
    /// Getter function pointer.
    GetFunctionPtr _get;
    /// Setter function pointer.
    SetFunctionPtr _set;
};

/// Template implementation for accessing serializable variables via functions where the setter uses reference, but the getter does not.
template <typename _Ty, typename U> class TMixedRefAttributeAccessorImpl : public IAttributeAccessor
{
public:
    typedef U (_Ty::*GetFunctionPtr)() const;
    typedef void (_Ty::*SetFunctionPtr)(const U&);

    /// Set new value for the variable.
    TMixedRefAttributeAccessorImpl(GetFunctionPtr getPtr, SetFunctionPtr setPtr) :
        _get(getPtr),
        _set(setPtr)
    {
        assert(_get);
        assert(_set);
    }

    /// Get current value of the variable.
    void Get(const ASerializable* instance, void* dest) override
    {
        assert(instance);

        U& value = *(reinterpret_cast<U*>(dest));
        const _Ty* classPtr = static_cast<const _Ty*>(instance);
        value = (classPtr->*_get)();
    }

    /// Set new value for the variable.
    void Set(ASerializable* instance, const void* source) override
    {
        assert(instance);

        const U& value = *(reinterpret_cast<const U*>(source));
        _Ty* classPtr = static_cast<_Ty*>(instance);
        (classPtr->*_set)(value);
    }

private:
    /// Getter function pointer.
    GetFunctionPtr _get;
    /// Setter function pointer.
    SetFunctionPtr _set;
};

}
