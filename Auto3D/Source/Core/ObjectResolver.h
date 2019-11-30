#pragma once

#include "Container/HashMap.h"

namespace Auto3D
{

class FAttribute;
class ASerializable;
struct FObjectRef;

/// Stored object ref attribute.
struct AUTO_API FStoredObjectRef
{
    /// Construct undefined.
    FStoredObjectRef() :
        _object(nullptr),
        _attr(nullptr),
        _oldId(0)
    {
    }

    /// Construct with values.
    FStoredObjectRef(ASerializable* object, FAttribute* attr, unsigned oldId) :
        _object(object),
        _attr(attr),
        _oldId(oldId)
    {
    }

    /// %AObject that contains the attribute.
    ASerializable* _object;
    /// Description of the object ref attribute.
    FAttribute* _attr;
    /// Old _id from the serialized data.
    unsigned _oldId;
};

/// Helper class for resolving object ref attributes when loading a scene.
class AUTO_API FObjectResolver
{
public:
    /// Store an object along with its old _id from the serialized data.
    void StoreObject(unsigned oldId, ASerializable* object);
    /// Store an object ref attribute that needs to be resolved later.
    void StoreObjectRef(ASerializable* object, FAttribute* attr, const FObjectRef& value);
    /// Resolve the object ref attributes.
    void Resolve();

private:
    /// Mapping of old _id's to objects.
    THashMap<unsigned, ASerializable*> _objects;
    /// Stored object ref attributes.
    TVector<FStoredObjectRef> _objectRefs;
};

}
