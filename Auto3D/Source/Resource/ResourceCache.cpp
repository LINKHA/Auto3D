#include "../Debug/Log.h"
#include "../Debug/Profiler.h"
#include "../IO/File.h"
#include "../IO/FileSystem.h"
#include "Image.h"
#include "JSONFile.h"
#include "ResourceCache.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

ResourceCache::ResourceCache()
{
    RegisterSubsystem(this);
}

ResourceCache::~ResourceCache()
{
    UnloadAllResources(true);
    RemoveSubsystem(this);
}

bool ResourceCache::AddResourceDir(const String& pathName, bool addFirst)
{
    PROFILE(AddResourceDir);

    if (!DirExists(pathName))
    {
        ErrorString("Could not open directory " + pathName);
        return false;
    }

    String fixedPath = SanitateResourceDirName(pathName);

    // Check that the same path does not already exist
    for (size_t i = 0; i < _resourceDirs.Size(); ++i)
    {
        if (!_resourceDirs[i].Compare(fixedPath, false))
            return true;
    }

    if (addFirst)
        _resourceDirs.Insert(0, fixedPath);
    else
        _resourceDirs.Push(fixedPath);

    InfoString("Added resource path " + fixedPath);
    return true;
}

bool ResourceCache::AddManualResource(Resource* resource)
{
    if (!resource)
    {
        ErrorString("Null manual resource");
        return false;
    }

    if (resource->Name().IsEmpty())
    {
        ErrorString("Manual resource with empty name, can not add");
        return false;
    }

    _resources[MakePair(resource->Type(), StringHash(resource->Name()))] = resource;
    return true;
}

void ResourceCache::RemoveResourceDir(const String& pathName)
{
    // Convert path to absolute
    String fixedPath = SanitateResourceDirName(pathName);

    for (size_t i = 0; i < _resourceDirs.Size(); ++i)
    {
        if (!_resourceDirs[i].Compare(fixedPath, false))
        {
            _resourceDirs.Erase(i);
            InfoString("Removed resource path " + fixedPath);
            return;
        }
    }
}

void ResourceCache::UnloadResource(StringHash type, const String& name, bool force)
{
    auto key = MakePair(type, StringHash(name));
    auto it = _resources.Find(key);
    if (it == _resources.End())
        return;

    Resource* resource = it->_second;
    if (resource->Refs() == 1 || force)
        _resources.Erase(key);
}

void ResourceCache::UnloadResources(StringHash type, bool force)
{
    // In case resources refer to other resources, _repeat until there are no further unloads
    for (;;)
    {
        size_t unloaded = 0;

        for (auto it = _resources.Begin(); it != _resources.End();)
        {
            auto current = it++;
            if (current->_first._first == type)
            {
                Resource* resource = current->_second;
                if (resource->Refs() == 1 || force)
                {
                    _resources.Erase(current);
                    ++unloaded;
                }
            }
        }

        if (!unloaded)
            break;
    }
}

void ResourceCache::UnloadResources(StringHash type, const String& partialName, bool force)
{
    // In case resources refer to other resources, _repeat until there are no further unloads
    for (;;)
    {
        size_t unloaded = 0;

        for (auto it = _resources.Begin(); it != _resources.End();)
        {
            auto current  = it++;
            if (current->_first._first == type)
            {
                Resource* resource = current->_second;
                if (resource->Name().StartsWith(partialName) && (resource->Refs() == 1 || force))
                {
                    _resources.Erase(current);
                    ++unloaded;
                }
            }
        }

        if (!unloaded)
            break;
    }
}

void ResourceCache::UnloadResources(const String& partialName, bool force)
{
    // In case resources refer to other resources, _repeat until there are no further unloads
    for (;;)
    {
        size_t unloaded = 0;

        for (auto it = _resources.Begin(); it != _resources.End();)
        {
            auto current = it++;
            Resource* resource = current->_second;
            if (resource->Name().StartsWith(partialName) && (!resource->Refs() == 1 || force))
            {
                _resources.Erase(current);
                ++unloaded;
            }
        }

        if (!unloaded)
            break;
    }
}

void ResourceCache::UnloadAllResources(bool force)
{
    // In case resources refer to other resources, _repeat until there are no further unloads
    for (;;)
    {
        size_t unloaded = 0;

        for (auto it = _resources.Begin(); it != _resources.End();)
        {
            auto current = it++;
            Resource* resource = current->_second;
            if (resource->Refs() == 1 || force)
            {
                _resources.Erase(current);
                ++unloaded;
            }
        }

        if (!unloaded)
            break;
    }
}

bool ResourceCache::ReloadResource(Resource* resource)
{
    if (!resource)
        return false;

    AutoPtr<Stream> stream = OpenResource(resource->Name());
    return stream ? resource->Load(*stream) : false;
}

AutoPtr<Stream> ResourceCache::OpenResource(const String& nameIn)
{
    String name = SanitateResourceName(nameIn);
    AutoPtr<Stream> ret;

    for (size_t i = 0; i < _resourceDirs.Size(); ++i)
    {
        if (FileExists(_resourceDirs[i] + name))
        {
            // Construct the file first with full path, then rename it to not contain the resource path,
            // so that the file's name can be used in further OpenResource() calls (for example over the network)
            ret = new File(_resourceDirs[i] + name);
            break;
        }
    }

    // Fallback using absolute path
    if (!ret)
        ret = new File(name);

    if (!ret->IsReadable())
    {
        ErrorString("Could not open resource file " + name);
        ret.Reset();
    }

    return ret;
}

Resource* ResourceCache::LoadResource(StringHash type, const String& nameIn)
{
    String name = SanitateResourceName(nameIn);

    // If empty name, return null pointer immediately without logging an error
    if (name.IsEmpty())
        return nullptr;

    // Check for existing resource
    auto key = MakePair(type, StringHash(name));
    auto it = _resources.Find(key);
    if (it != _resources.End())
        return it->_second;

    SharedPtr<Object> newObject = Create(type);
    if (!newObject)
    {
        ErrorString("Could not load unknown resource type " + String(type));
        return nullptr;
    }
    Resource* newResource = dynamic_cast<Resource*>(newObject.Get());
    if (!newResource)
    {
        ErrorString("Type " + String(type) + " is not a resource");
        return nullptr;
    }

    // Attempt to load the resource
    AutoPtr<Stream> stream = OpenResource(name);
    if (!stream)
        return nullptr;

    LogString("Loading resource " + name);
    newResource->SetName(name);
    if (!newResource->Load(*stream))
        return nullptr;

    // Store to cache
    _resources[key] = newResource;
    return newResource;
}

void ResourceCache::ResourcesByType(Vector<Resource*>& result, StringHash type) const
{
    result.Clear();

    for (auto it = _resources.Begin(); it != _resources.End(); ++it)
    {
        if (it->_second->Type() == type)
            result.Push(it->_second);
    }
}

bool ResourceCache::Exists(const String& nameIn) const
{
    String name = SanitateResourceName(nameIn);

    for (size_t i = 0; i < _resourceDirs.Size(); ++i)
    {
        if (FileExists(_resourceDirs[i] + name))
            return true;
    }

    // Fallback using absolute path
    return FileExists(name);
}

String ResourceCache::ResourceFileName(const String& name) const
{
    for (unsigned i = 0; i < _resourceDirs.Size(); ++i)
    {
        if (FileExists(_resourceDirs[i] + name))
            return _resourceDirs[i] + name;
    }

    return String();
}

String ResourceCache::SanitateResourceName(const String& nameIn) const
{
    // Sanitate unsupported constructs from the resource name
    String name = NormalizePath(nameIn);
    name.Replace("../", "");
    name.Replace("./", "");

    // If the path refers to one of the resource directories, normalize the resource name
    if (_resourceDirs.Size())
    {
        String namePath = Path(name);
        String exePath = ExecutableDir();
        for (unsigned i = 0; i < _resourceDirs.Size(); ++i)
        {
            String relativeResourcePath = _resourceDirs[i];
            if (relativeResourcePath.StartsWith(exePath))
                relativeResourcePath = relativeResourcePath.Substring(exePath.Length());

            if (namePath.StartsWith(_resourceDirs[i], false))
                namePath = namePath.Substring(_resourceDirs[i].Length());
            else if (namePath.StartsWith(relativeResourcePath, false))
                namePath = namePath.Substring(relativeResourcePath.Length());
        }

        name = namePath + FileNameAndExtension(name);
    }

    return name.Trimmed();
}

String ResourceCache::SanitateResourceDirName(const String& nameIn) const
{
    // Convert path to absolute
    String fixedPath = AddTrailingSlash(nameIn);
    if (!IsAbsolutePath(fixedPath))
        fixedPath = CurrentDir() + fixedPath;

    // Sanitate away /./ construct
    fixedPath.Replace("/./", "/");

    return fixedPath.Trimmed();
}

void RegisterResourceLibrary()
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    Image::RegisterObject();
    JSONFile::RegisterObject();
}

}
