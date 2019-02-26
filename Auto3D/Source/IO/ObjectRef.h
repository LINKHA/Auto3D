#pragma once

#include "../AutoConfig.h"

namespace Auto3D
{

/// Reference to an object with _id for serialization.
struct AUTO_API ObjectRef
{
    /// %Object _id.
    unsigned _id;

    /// Construct with no reference.
    ObjectRef() :
        _id(0)
    {
    }

    // Copy-construct.
    ObjectRef(const ObjectRef& ref) :
        _id(ref._id)
    {
    }

    /// Construct with object _id.
    ObjectRef(unsigned id_) :
        _id(id_)
    {
    }

    /// Test for equality with another reference.
    bool operator == (const ObjectRef& rhs) const { return _id == rhs._id; }
    /// Test for inequality with another reference.
    bool operator != (const ObjectRef& rhs) const { return !(*this == rhs); }
};

}
