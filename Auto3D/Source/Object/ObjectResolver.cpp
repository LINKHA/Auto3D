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
        _objects[oldId] = object;
}

void ObjectResolver::StoreObjectRef(Serializable* object, Attribute* attr, const ObjectRef& value)
{
    if (object && attr && attr->Type() == AttributeType::OBJECTREF)
        _objectRefs.Push(StoredObjectRef(object, attr, value._id));
}

void ObjectResolver::Resolve()
{
    for (auto it = _objectRefs.Begin(); it != _objectRefs.End(); ++it)
    {
        auto refIt = _objects.Find(it->_oldId);
        // See if we can find the referred to object
        if (refIt != _objects.End())
        {
            AttributeImpl<ObjectRef>* typedAttr = static_cast<AttributeImpl<ObjectRef>*>(it->_attr);
            typedAttr->SetValue(it->_object, ObjectRef(refIt->_second->Id()));
        }
        else
            WarinningString("Could not resolve object reference " + String(it->_oldId));
    }
}

}
