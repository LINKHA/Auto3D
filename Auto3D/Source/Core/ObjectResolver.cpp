#include "Debug/Log.h"
#include "IO/ObjectRef.h"
#include "ObjectResolver.h"
#include "Serializable.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

void FObjectResolver::StoreObject(unsigned oldId, ASerializable* object)
{
    if (object)
        _objects[oldId] = object;
}

void FObjectResolver::StoreObjectRef(ASerializable* object, FAttribute* attr, const ObjectRef& value)
{
    if (object && attr && attr->Type() == EAttributeType::OBJECTREF)
        _objectRefs.Push(FStoredObjectRef(object, attr, value._id));
}

void FObjectResolver::Resolve()
{
    for (auto it = _objectRefs.Begin(); it != _objectRefs.End(); ++it)
    {
        auto refIt = _objects.Find(it->_oldId);
        // See if we can find the referred to object
        if (refIt != _objects.End())
        {
            FAttributeImpl<ObjectRef>* typedAttr = static_cast<FAttributeImpl<ObjectRef>*>(it->_attr);
            typedAttr->SetValue(it->_object, ObjectRef(refIt->_second->Id()));
        }
        else
            WarningString("Could not resolve object reference " + FString(it->_oldId));
    }
}

}
