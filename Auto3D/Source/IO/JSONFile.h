#pragma once

#include "IO/JSONValue.h"
#include "Resource.h"

namespace Auto3D
{

class FStream;

/// JSON document. Contains a root JSON value and can be read/written to file as text.
class AUTO_API AJSONFile : public OResource
{
    DECLARE_A_CLASS(AJSONFile, OResource)

public:
    /// Load from a stream as text. Return true on success. Will contain partial data on failure.
    bool BeginLoad(FStream& source) override;
    /// Save to a stream as text. Return true on success.
    bool Save(FStream& dest) override;

    /// Return the root value.
    FJSONValue& Root() { return _root; }
    /// Return the const root value.
    const FJSONValue& Root() const { return _root; }

private:
    /// Root value.
    FJSONValue _root;
};

}
