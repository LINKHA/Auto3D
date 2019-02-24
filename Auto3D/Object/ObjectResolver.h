#pragma once

#include "../Base/HashMap.h"

namespace Auto3D
{

class Attribute;
class Serializable;
struct ObjectRef;

/// Stored object ref attribute.
struct AUTO_API StoredObjectRef
{
    /// Construct undefined.
    StoredObjectRef() :
        object(nullptr),
        attr(nullptr),
        oldId(0)
    {
    }

    /// Construct with values.
    StoredObjectRef(Serializable* object_, Attribute* attr_, unsigned oldId_) :
        object(object_),
        attr(attr_),
        oldId(oldId_)
    {
    }

    /// %Object that contains the attribute.
    Serializable* object;
    /// Description of the object ref attribute.
    Attribute* attr;
    /// Old id from the serialized data.
    unsigned oldId;
};

/// Helper class for resolving object ref attributes when loading a scene.
class AUTO_API ObjectResolver
{
public:
    /// Store an object along with its old id from the serialized data.
    void StoreObject(unsigned oldId, Serializable* object);
    /// Store an object ref attribute that needs to be resolved later.
    void StoreObjectRef(Serializable* object, Attribute* attr, const ObjectRef& value);
    /// Resolve the object ref attributes.
    void Resolve();

private:
    /// Mapping of old id's to objects.
    HashMap<unsigned, Serializable*> objects;
    /// Stored object ref attributes.
    Vector<StoredObjectRef> objectRefs;
};

}
