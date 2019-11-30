#pragma once

#include "AutoConfig.h"

namespace Auto3D
{

/// Reference to an object with id for serialization.
struct AUTO_API FObjectRef
{
    /// %AObject _id.
    unsigned _id;

    /// Construct with no reference.
    FObjectRef() :
        _id(0)
    {
    }

    // Copy-construct.
    FObjectRef(const FObjectRef& ref) :
        _id(ref._id)
    {
    }

    /// Construct with object _id.
    FObjectRef(unsigned id_) :
        _id(id_)
    {
    }

    /// Test for equality with another reference.
    bool operator == (const FObjectRef& rhs) const { return _id == rhs._id; }
    /// Test for inequality with another reference.
    bool operator != (const FObjectRef& rhs) const { return !(*this == rhs); }
};

}
