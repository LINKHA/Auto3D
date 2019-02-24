#include "../Debug/Log.h"
#include "../IO/ObjectRef.h"
#include "ObjectResolver.h"
#include "Serializable.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

void ObjectResolver::StoreObject(unsigned oldId, Serializable* object)
{
    if (object)
        objects[oldId] = object;
}

void ObjectResolver::StoreObjectRef(Serializable* object, Attribute* attr, const ObjectRef& value)
{
    if (object && attr && attr->Type() == ATTR_OBJECTREF)
        objectRefs.Push(StoredObjectRef(object, attr, value.id));
}

void ObjectResolver::Resolve()
{
    for (auto it = objectRefs.Begin(); it != objectRefs.End(); ++it)
    {
        auto refIt = objects.Find(it->oldId);
        // See if we can find the referred to object
        if (refIt != objects.End())
        {
            AttributeImpl<ObjectRef>* typedAttr = static_cast<AttributeImpl<ObjectRef>*>(it->attr);
            typedAttr->SetValue(it->object, ObjectRef(refIt->second->Id()));
        }
        else
            LOGWARNING("Could not resolve object reference " + String(it->oldId));
    }
}

}
