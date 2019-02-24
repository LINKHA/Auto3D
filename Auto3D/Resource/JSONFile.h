#pragma once

#include "../IO/JSONValue.h"
#include "Resource.h"

namespace Auto3D
{

class Stream;

/// JSON document. Contains a root JSON value and can be read/written to file as text.
class AUTO_API JSONFile : public Resource
{
    REGISTER_OBJECT_CLASS(JSONFile, Resource)

public:
    /// Load from a stream as text. Return true on success. Will contain partial data on failure.
    bool BeginLoad(Stream& source) override;
    /// Save to a stream as text. Return true on success.
    bool Save(Stream& dest) override;
    
    /// Register object factory.
    static void RegisterObject();

    /// Return the root value.
    JSONValue& Root() { return root; }
    /// Return the const root value.
    const JSONValue& Root() const { return root; }

private:
    /// Root value.
    JSONValue root;
};

}
