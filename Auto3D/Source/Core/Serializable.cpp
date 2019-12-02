#include "IO/JSONValue.h"
#include "IO/ObjectRef.h"
#include "IO/Stream.h"
#include "ObjectResolver.h"
#include "Serializable.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

THashMap<FStringHash, TVector<TSharedPtr<FAttribute> > > ASerializable::_classAttributes;

void ASerializable::Load(FStream& source, FObjectResolver& resolver)
{
    const TVector<TSharedPtr<FAttribute> >* attributes = Attributes();
    if (!attributes)
        return; // Nothing to do
    
    size_t numAttrs = source.ReadVLE();
    for (size_t i = 0; i < numAttrs; ++i)
    {
        // Skip attribute if wrong type or extra data
        EAttributeType::Type type = (EAttributeType::Type)source.Read<unsigned char>();
        bool skip = true;
        
        if (i < attributes->Size())
        {
            FAttribute* attr = attributes->At(i);
            if (attr->Type() == type)
            {
                // Store object refs to the resolver instead of immediately setting
                if (type != EAttributeType::OBJECTREF)
                    attr->FromBinary(this, source);
                else
                    resolver.StoreObjectRef(this, attr, source.Read<FObjectRef>());
                
                skip = false;
            }
        }
        
        if (skip)
            FAttribute::Skip(type, source);
    }
}

void ASerializable::Save(FStream& dest)
{
    const TVector<TSharedPtr<FAttribute> >* attributes = Attributes();
    if (!attributes)
        return;
    
    dest.WriteVLE(attributes->Size());
    for (auto it = attributes->Begin(); it != attributes->End(); ++it)
    {
        FAttribute* attr = *it;
        dest.Write<unsigned char>((unsigned char)attr->Type());
        attr->ToBinary(this, dest);
    }
}

void ASerializable::LoadJSON(const FJSONValue& source, FObjectResolver& resolver)
{
    const TVector<TSharedPtr<FAttribute> >* attributes = Attributes();
    if (!attributes || !source.IsObject() || !source.Size())
        return;
    
    const JSONObject& object = source.GetObject();
    
    for (auto it = attributes->Begin(); it != attributes->End(); ++it)
    {
        FAttribute* attr = *it;
        auto jsonIt = object.Find(attr->GetName());
        if (jsonIt != object.End())
        {
            // Store object refs to the resolver instead of immediately setting
            if (attr->Type() != EAttributeType::OBJECTREF)
                attr->FromJSON(this, jsonIt->_second);
            else
                resolver.StoreObjectRef(this, attr, FObjectRef((unsigned)jsonIt->_second.GetNumber()));
        }
    }
}

void ASerializable::SaveJSON(FJSONValue& dest)
{
    const TVector<TSharedPtr<FAttribute> >* attributes = Attributes();
    if (!attributes)
        return;
    
    for (size_t i = 0; i < attributes->Size(); ++i)
    {
        FAttribute* attr = attributes->At(i);
        // For better readability, do not save default-valued attributes to JSON
        if (!attr->IsDefault(this))
            attr->ToJSON(this, dest[attr->GetName()]);
    }
}

void ASerializable::SetAttributeValue(FAttribute* attr, const void* source)
{
    if (attr)
        attr->FromValue(this, source);
}

void ASerializable::AttributeValue(FAttribute* attr, void* dest)
{
    if (attr)
        attr->ToValue(this, dest);
}

const TVector<TSharedPtr<FAttribute> >* ASerializable::Attributes() const
{
    auto it = _classAttributes.Find(GetType());
    return it != _classAttributes.End() ? &it->_second : nullptr;
}

FAttribute* ASerializable::FindAttribute(const FString& name) const
{
    return FindAttribute(name.CString());
}

FAttribute* ASerializable::FindAttribute(const char* name) const
{
    const TVector<TSharedPtr<FAttribute> >* attributes = Attributes();
    if (!attributes)
        return nullptr;
    
    for (size_t i = 0; i < attributes->Size(); ++i)
    {
        FAttribute* attr = attributes->At(i);
        if (attr->GetName() == name)
            return attr;
    }
    
    return nullptr;
}

void ASerializable::RegisterAttribute(FStringHash type, FAttribute* attr)
{
    TVector<TSharedPtr<FAttribute> >& attributes = _classAttributes[type];
    for (size_t i = 0; i < attributes.Size(); ++i)
    {
        if (attributes[i]->GetName() == attr->GetName())
        {
            attributes.Insert(i, attr);
            return;
        }
    }
    
    attributes.Push(attr);
}

void ASerializable::CopyBaseAttributes(FStringHash type, FStringHash baseType)
{
    // Make sure the types are different, which may not be true if the OBJECT macro has been omitted
    if (type != baseType)
    {
        TVector<TSharedPtr<FAttribute> >& attributes = _classAttributes[baseType];
        for (size_t i = 0; i < attributes.Size(); ++i)
            RegisterAttribute(type, attributes[i]);
    }
}

void ASerializable::CopyBaseAttribute(FStringHash type, FStringHash baseType, const FString& name)
{
    // Make sure the types are different, which may not be true if the OBJECT macro has been omitted
    if (type != baseType)
    {
        TVector<TSharedPtr<FAttribute> >& attributes = _classAttributes[baseType];
        for (size_t i = 0; i < attributes.Size(); ++i)
        {
            if (attributes[i]->GetName() == name)
            {
                RegisterAttribute(type, attributes[i]);
                break;
            }
        }
    }
}

void ASerializable::Skip(FStream& source)
{
    size_t numAttrs = source.ReadVLE();
    for (size_t i = 0; i < numAttrs; ++i)
    {
        EAttributeType::Type type = (EAttributeType::Type)source.Read<unsigned char>();
        FAttribute::Skip(type, source);
    }
}

}
