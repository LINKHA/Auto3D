#pragma once

#include "../Base/AutoPtr.h"
#include "../Base/Ptr.h"
#include "../IO/Stream.h"

namespace Auto3D
{

class JSONValue;
class Serializable;
class Stream;

/// Supported attribute types.
enum AttributeType
{
    ATTR_BOOL = 0,
    ATTR_BYTE,
    ATTR_UNSIGNED,
    ATTR_INT,
    ATTR_INTVECTOR2,
    ATTR_INTRECT,
    ATTR_FLOAT,
    ATTR_VECTOR2,
    ATTR_VECTOR3,
    ATTR_VECTOR4,
    ATTR_QUATERNION,
    ATTR_COLOR,
    ATTR_RECT,
    ATTR_BOUNDINGBOX,
    ATTR_MATRIX3,
    ATTR_MATRIX3X4,
    ATTR_MATRIX4,
    ATTR_STRING,
    ATTR_RESOURCEREF,
    ATTR_RESOURCEREFLIST,
    ATTR_OBJECTREF,
    ATTR_JSONVALUE,
    MAX_ATTR_TYPES
};

/// Helper class for accessing serializable variables via getter and setter functions.
class AUTO_API AttributeAccessor
{
public:
    /// Destruct.
    virtual ~AttributeAccessor();
    
    /// Get the current value of the variable.
    virtual void Get(const Serializable* instance, void* dest) = 0;
    /// Set new value for the variable.
    virtual void Set(Serializable* instance, const void* source) = 0;
};

/// Description of an automatically serializable variable.
class AUTO_API Attribute : public RefCounted
{
public:
    /// Construct.
    Attribute(const char* name, AttributeAccessor* accessor, const char** enumNames = 0);
    
    /// Deserialize from a binary stream.
    virtual void FromBinary(Serializable* instance, Stream& source) = 0;
    /// Serialize to a binary stream.
    virtual void ToBinary(Serializable* instance, Stream& dest) = 0;
    /// Deserialize from JSON.
    virtual void FromJSON(Serializable* instance, const JSONValue& source) = 0;
    /// Serialize to JSON.
    virtual void ToJSON(Serializable* instance, JSONValue& dest) = 0;
    /// Return type.
    virtual AttributeType Type() const = 0;
    /// Return whether is default value.
    virtual bool IsDefault(Serializable* instance) = 0;
    
    /// Set from a value in memory.
    void FromValue(Serializable* instance, const void* source);
    /// Copy to a value in memory.
    void ToValue(Serializable* instance, void* dest);
    
    /// Return variable name.
    const String& Name() const { return name; }
    /// Return zero-based enum names, or null if none.
    const char** EnumNames() const { return enumNames; }
    /// Return type name.
    const String& TypeName() const;
    /// Return byte size of the attribute data, or 0 if it can be variable.
    size_t ByteSize() const;
    
    /// Skip binary data of an attribute.
    static void Skip(AttributeType type, Stream& source);
    /// Serialize attribute value to JSON.
    static void ToJSON(AttributeType type, JSONValue& dest, const void* source);
    /// Deserialize attribute value from JSON.
    static void FromJSON(AttributeType type, void* dest, const JSONValue& source);
    /// Return attribute type from type name.
    static AttributeType TypeFromName(const String& name);
    /// Return attribute type from type name.
    static AttributeType TypeFromName(const char* name);
    
    /// Type names.
    static const String typeNames[];
    /// Attribute byte sizes.
    static const size_t byteSizes[];

protected:
    /// Variable name.
    String name;
    /// Attribute accessor.
    AutoPtr<AttributeAccessor> accessor;
    /// Enum names.
    const char** enumNames;

private:
    /// Prevent copy construction.
    Attribute(const Attribute& rhs);
    /// Prevent assignment.
    Attribute& operator = (const Attribute& rhs);
};

/// Template implementation of an attribute description with specific type.
template <class _Ty> class AttributeImpl : public Attribute
{
public:
    /// Construct.
    AttributeImpl(const char* name, AttributeAccessor* accessor, const _Ty& defaultValue_, const char** enumNames = 0) :
        Attribute(name, accessor, enumNames),
        defaultValue(defaultValue_)
    {
    }
    
    /// Deserialize from a binary stream.
    void FromBinary(Serializable* instance, Stream& source) override
    {
        _Ty value = source.Read<_Ty>();
        accessor->Set(instance, &value);
    }
    
    /// Serialize to a binary stream.
    void ToBinary(Serializable* instance, Stream& dest) override
    {
        _Ty value;
        accessor->Get(instance, &value);
        dest.Write<_Ty>(value);
    }
    
    /// Return whether is default value.
    bool IsDefault(Serializable* instance) override { return Value(instance) == defaultValue; }
    
    /// Deserialize from JSON.
    void FromJSON(Serializable* instance, const JSONValue& source) override
    {
        _Ty value;
        Attribute::FromJSON(Type(), &value, source);
        accessor->Set(instance, &value);
    }

    /// Serialize to JSON.
    void ToJSON(Serializable* instance, JSONValue& dest) override
    {
        _Ty value;
        accessor->Get(instance, &value);
        Attribute::ToJSON(Type(), dest, &value);
    }

    /// Return type.
    AttributeType Type() const override;
    
    /// Set new attribute value.
    void SetValue(Serializable* instance, const _Ty& source) { accessor->Set(instance, &source); }
    /// Copy current attribute value.
    void Value(Serializable* instance, _Ty& dest) { accessor->Get(instance, &dest); }
    
    /// Return current attribute value.
    _Ty Value(Serializable* instance)
    {
        _Ty ret;
        accessor->Get(instance, &ret);
        return ret;
    }
    
    /// Return default value.
    const _Ty& DefaultValue() const { return defaultValue; }
    
private:
    /// Default value.
    _Ty defaultValue;
};

/// Template implementation for accessing serializable variables.
template <class _Ty, class U> class AttributeAccessorImpl : public AttributeAccessor
{
public:
    typedef U (_Ty::*GetFunctionPtr)() const;
    typedef void (_Ty::*SetFunctionPtr)(U);

    /// Construct with function pointers.
    AttributeAccessorImpl(GetFunctionPtr getPtr, SetFunctionPtr setPtr) :
        get(getPtr),
        set(setPtr)
    {
        assert(get);
        assert(set);
    }

    /// Get current value of the variable.
    void Get(const Serializable* instance, void* dest) override
    {
        assert(instance);

        U& value = *(reinterpret_cast<U*>(dest));
        const _Ty* classInstance = static_cast<const _Ty*>(instance);
        value = (classInstance->*get)();
    }

    /// Set new value for the variable.
    void Set(Serializable* instance, const void* source) override
    {
        assert(instance);

        const U& value = *(reinterpret_cast<const U*>(source));
        _Ty* classInstance = static_cast<_Ty*>(instance);
        (classInstance->*set)(value);
    }

private:
    /// Getter function pointer.
    GetFunctionPtr get;
    /// Setter function pointer.
    SetFunctionPtr set;
};

/// Template implementation for accessing serializable variables via functions that use references.
template <class _Ty, class U> class RefAttributeAccessorImpl : public AttributeAccessor
{
public:
    typedef const U& (_Ty::*GetFunctionPtr)() const;
    typedef void (_Ty::*SetFunctionPtr)(const U&);

    /// Set new value for the variable.
    RefAttributeAccessorImpl(GetFunctionPtr getPtr, SetFunctionPtr setPtr) :
        get(getPtr),
        set(setPtr)
    {
        assert(get);
        assert(set);
    }

    /// Get current value of the variable.
    void Get(const Serializable* instance, void* dest) override
    {
        assert(instance);

        U& value = *(reinterpret_cast<U*>(dest));
        const _Ty* classPtr = static_cast<const _Ty*>(instance);
        value = (classPtr->*get)();
    }

    /// Set new value for the variable.
    void Set(Serializable* instance, const void* source) override
    {
        assert(instance);

        const U& value = *(reinterpret_cast<const U*>(source));
        _Ty* classPtr = static_cast<_Ty*>(instance);
        (classPtr->*set)(value);
    }

private:
    /// Getter function pointer.
    GetFunctionPtr get;
    /// Setter function pointer.
    SetFunctionPtr set;
};

/// Template implementation for accessing serializable variables via functions where the setter uses reference, but the getter does not.
template <class _Ty, class U> class MixedRefAttributeAccessorImpl : public AttributeAccessor
{
public:
    typedef U (_Ty::*GetFunctionPtr)() const;
    typedef void (_Ty::*SetFunctionPtr)(const U&);

    /// Set new value for the variable.
    MixedRefAttributeAccessorImpl(GetFunctionPtr getPtr, SetFunctionPtr setPtr) :
        get(getPtr),
        set(setPtr)
    {
        assert(get);
        assert(set);
    }

    /// Get current value of the variable.
    void Get(const Serializable* instance, void* dest) override
    {
        assert(instance);

        U& value = *(reinterpret_cast<U*>(dest));
        const _Ty* classPtr = static_cast<const _Ty*>(instance);
        value = (classPtr->*get)();
    }

    /// Set new value for the variable.
    void Set(Serializable* instance, const void* source) override
    {
        assert(instance);

        const U& value = *(reinterpret_cast<const U*>(source));
        _Ty* classPtr = static_cast<_Ty*>(instance);
        (classPtr->*set)(value);
    }

private:
    /// Getter function pointer.
    GetFunctionPtr get;
    /// Setter function pointer.
    SetFunctionPtr set;
};

}
