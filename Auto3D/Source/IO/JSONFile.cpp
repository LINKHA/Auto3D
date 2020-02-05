#include "Container/AutoPtr.h"
#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "IO/File.h"
#include "JSONFile.h"
#include "Resource/ResourceCache.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

bool OJSONFile::BeginLoad(const FString& pathName)
{
    PROFILE(LoadJSONFile);
	GResourceModule& resourceCahce = GResourceModule::Get();
	UPtr<FStream> stream(resourceCahce.OpenResource(pathName));
	if (!stream)
		return false;
	FStream& source = *stream;

    size_t dataSize = source.Size() - source.Position();
    TAutoArrayPtr<char> buffer(new char[dataSize]);
    if (source.Read(buffer.Get(), dataSize) != dataSize)
        return false;
    
    const char* pos = buffer.Get();
    const char* end = pos + dataSize;
    
    // Remove any previous content
    _root.SetNull();
    /// \todo If fails, log the line number on which the error occurred
    bool success = _root.Parse(pos, end);
    if (!success)
    {
        ErrorString("Parsing JSON from " + source.GetPathName() + " failed; data may be partial");
    }

    return success;
}

bool OJSONFile::Save(FStream& dest)
{
    PROFILE(SaveJSONFile);
    
    FString buffer;
    _root.ToString(buffer);
    return dest.Write(buffer.Begin()._ptr, buffer.Length()) == buffer.Length();
}

}
