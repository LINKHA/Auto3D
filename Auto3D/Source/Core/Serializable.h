#pragma once

#include "Attribute.h"
#include "Object.h"

namespace Auto3D
{

class FObjectResolver;

/// Base class for objects with automatic serialization using attributes.
class AUTO_API ASerializable : public AObject
{
public:
	/// Destructor
	virtual ~ASerializable() {}
    /// Load from binary stream. Store object ref attributes to be resolved later.
    virtual void Load(FStream& source, FObjectResolver& resolver);
    /// Save to binary stream.
    virtual void Save(FStream& dest);
    /// Load from JSON data. Optionally store object ref attributes to be resolved later.
    virtual void LoadJSON(const FJSONValue& source, FObjectResolver& resolver);
    /// Save as JSON data.
    virtual void SaveJSON(FJSONValue& dest);
    /// Return _id for referring to the object in serialization.
    virtual unsigned Id() const { return 0; }

    /// Set attribute value from memory.
    void SetAttributeValue(FAttribute* attr, const void* source);
    /// Copy attribute value to memory.
    void AttributeValue(FAttribute* attr, void* dest);
    
    /// Set attribute value, template version. Return true if value was right type.
    template <typename _Ty> bool SetAttributeValue(FAttribute* attr, const _Ty& source)
    {
        FAttributeImpl<_Ty>* typedAttr = dynamic_cast<FAttributeImpl<_Ty>*>(attr);
        if (typedAttr)
        {
            typedAttr->SetValue(this, source);
            return true;
        }
        else
            return false;
    }
    
    /// Copy attribute value, template version. Return true if value was right type.
    template <typename _Ty> bool AttributeValue(FAttribute* attr, _Ty& dest)
    {
        FAttributeImpl<_Ty>* typedAttr = dynamic_cast<FAttributeImpl<_Ty>*>(attr);
        if (typedAttr)
        {
            typedAttr->Value(this, dest);
            return true;
        }
        else
            return false;
    }
    
    /// Return attribute value, template version.
    template <typename _Ty> _Ty AttributeValue(FAttribute* attr)
    {
        FAttributeImpl<_Ty>* typedAttr = dynamic_cast<FAttributeImpl<_Ty>*>(attr);
        return typedAttr ? typedAttr->Value(this) : _Ty();
    }
    
    /// Return the attribute descriptions. Default implementation uses per-class registration.
    virtual const TVector<TSharedPtr<FAttribute> >* Attributes() const;
    /// Return an attribute description by name, or null if does not exist.
    FAttribute* FindAttribute(const FString& name) const;
    /// Return an attribute description by name, or null if does not exist.
    FAttribute* FindAttribute(const char* name) const;
    
    /// Register a per-class attribute. If an attribute with the same name already exists, it will be replaced.
    static void RegisterAttribute(FStringHash type, FAttribute* attr);
    /// Copy all base class attributes.
    static void CopyBaseAttributes(FStringHash type, FStringHash baseType);
    /// Copy one base class attribute.
    static void CopyBaseAttribute(FStringHash type, FStringHash baseType, const FString& name);
    /// Skip binary data of an object's all attributes.
    static void Skip(FStream& source);
    
    /// Register a per-class attribute, template version. Should not be used for base class attributes unless the type is explicitly specified, as by default the attribute will be re-registered to the base class redundantly.
    template <typename _Ty, typename U> static void RegisterAttribute(const char* name, U (_Ty::*getFunction)() const, void (_Ty::*setFunction)(U), const U& defaultValue = U(), const char** enumNames = 0)
    {
        RegisterAttribute(_Ty::GetTypeStatic(), new FAttributeImpl<U>(name, new FAttributeAccessorImpl<_Ty, U>(getFunction, setFunction), defaultValue, enumNames));
    }
    
    /// Register a per-class attribute with reference access, template version. Should not be used for base class attributes unless the type is explicitly specified, as by default the attribute will be re-registered to the base class redundantly.
    template <typename _Ty, typename U> static void RegisterRefAttribute(const char* name, const U& (_Ty::*getFunction)() const, void (_Ty::*setFunction)(const U&), const U& defaultValue = U(), const char** enumNames = 0)
    {
        RegisterAttribute(_Ty::GetTypeStatic(), new FAttributeImpl<U>(name, new FRefAttributeAccessorImpl<_Ty, U>(getFunction, setFunction), defaultValue, enumNames));
    }

    /// Register a per-class attribute with mixed reference access, template version. Should not be used for base class attributes unless the type is explicitly specified, as by default the attribute will be re-registered to the base class redundantly.
    template <typename _Ty, typename U> static void RegisterMixedRefAttribute(const char* name, U (_Ty::*getFunction)() const, void (_Ty::*setFunction)(const U&), const U& defaultValue = U(), const char** enumNames = 0)
    {
        RegisterAttribute(_Ty::GetTypeStatic(), new FAttributeImpl<U>(name, new FMixedRefAttributeAccessorImpl<_Ty, U>(getFunction, setFunction), defaultValue, enumNames));
    }

    /// Copy all base class attributes, template version.
    template <typename _Ty, typename U> static void CopyBaseAttributes()
    {
        CopyBaseAttributes(_Ty::GetTypeStatic(), U::GetTypeStatic());
    }

    /// Copy one base class attribute, template version.
    template <typename _Ty, typename U> static void CopyBaseAttribute(const FString& name)
    {
        CopyBaseAttribute(_Ty::GetTypeStatic(), U::GetTypeStatic(), name);
    }
    
private:
    /// Per-class attributes.
    static THashMap<FStringHash, TVector<TSharedPtr<FAttribute> > > _classAttributes;
};

}
