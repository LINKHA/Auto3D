#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "IO/File.h"
#include "IO/FileSystem.h"
#include "Image.h"
#include "JSONFile.h"
#include "ResourceCache.h"
#include "Font.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

ResourceCache::ResourceCache()
{
	AddResourceDir(ExecutableDir() + "Data");
}

ResourceCache::~ResourceCache()
{
    UnloadAllResources(true);
}

bool ResourceCache::AddResourceDir(const FString& pathName, bool addFirst)
{
    PROFILE(AddResourceDir);

    if (!DirExists(pathName))
    {
        ErrorString("Could not open directory " + pathName);
        return false;
    }

    FString fixedPath = SanitateResourceDirName(pathName);

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

bool ResourceCache::AddManualResource(AResource* resource)
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

    _resources[MakePair(resource->GetType(), FStringHash(resource->Name()))] = resource;
    return true;
}

void ResourceCache::RemoveResourceDir(const FString& pathName)
{
    // Convert path to absolute
    FString fixedPath = SanitateResourceDirName(pathName);

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

void ResourceCache::UnloadResource(FStringHash type, const FString& name, bool force)
{
    auto key = MakePair(type, FStringHash(name));
    auto it = _resources.Find(key);
    if (it == _resources.End())
        return;

    AResource* resource = it->_second;
    if (resource->Refs() == 1 || force)
        _resources.Erase(key);
}

void ResourceCache::UnloadResources(FStringHash type, bool force)
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
                AResource* resource = current->_second;
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

void ResourceCache::UnloadResources(FStringHash type, const FString& partialName, bool force)
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
                AResource* resource = current->_second;
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

void ResourceCache::UnloadResources(const FString& partialName, bool force)
{
    // In case resources refer to other resources, _repeat until there are no further unloads
    for (;;)
    {
        size_t unloaded = 0;

        for (auto it = _resources.Begin(); it != _resources.End();)
        {
            auto current = it++;
            AResource* resource = current->_second;
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
            AResource* resource = current->_second;
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

bool ResourceCache::ReloadResource(AResource* resource)
{
    if (!resource)
        return false;

    TAutoPtr<Stream> stream = OpenResource(resource->Name());
    return stream ? resource->Load(*stream) : false;
}

TAutoPtr<Stream> ResourceCache::OpenResource(const FString& nameIn)
{
    FString name = SanitateResourceName(nameIn);
    TAutoPtr<Stream> ret;

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

AResource* ResourceCache::LoadResource(FStringHash type, const FString& nameIn)
{
    FString name = SanitateResourceName(nameIn);

    // If empty name, return null pointer immediately without logging an error
    if (name.IsEmpty())
        return nullptr;

    // Check for existing resource
    auto key = MakePair(type, FStringHash(name));
    auto it = _resources.Find(key);
    if (it != _resources.End())
        return it->_second;

    TSharedPtr<AObject> newObject = AObject::Create(type);
    if (!newObject)
    {
        ErrorString("Could not load unknown resource type " + FString(type));
        return nullptr;
    }
    AResource* newResource = dynamic_cast<AResource*>(newObject.Get());
    if (!newResource)
    {
        ErrorString("Type " + FString(type) + " is not a resource");
        return nullptr;
    }

    // Attempt to load the resource
    TAutoPtr<Stream> stream = OpenResource(name);
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

void ResourceCache::ResourcesByType(TVector<AResource*>& result, FStringHash type) const
{
    result.Clear();

    for (auto it = _resources.Begin(); it != _resources.End(); ++it)
    {
        if (it->_second->GetType() == type)
            result.Push(it->_second);
    }
}

bool ResourceCache::Exists(const FString& nameIn) const
{
    FString name = SanitateResourceName(nameIn);

    for (size_t i = 0; i < _resourceDirs.Size(); ++i)
    {
        if (FileExists(_resourceDirs[i] + name))
            return true;
    }

    // Fallback using absolute path
    return FileExists(name);
}

FString ResourceCache::ResourceFileName(const FString& name) const
{
    for (unsigned i = 0; i < _resourceDirs.Size(); ++i)
    {
        if (FileExists(_resourceDirs[i] + name))
            return _resourceDirs[i] + name;
    }

    return FString();
}

FString ResourceCache::SanitateResourceName(const FString& nameIn) const
{
    // Sanitate unsupported constructs from the resource name
    FString name = NormalizePath(nameIn);
    name.Replace("../", "");
    name.Replace("./", "");

    // If the path refers to one of the resource directories, normalize the resource name
    if (_resourceDirs.Size())
    {
        FString namePath = Path(name);
        FString exePath = ExecutableDir();
        for (unsigned i = 0; i < _resourceDirs.Size(); ++i)
        {
            FString relativeResourcePath = _resourceDirs[i];
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

FString ResourceCache::SanitateResourceDirName(const FString& nameIn) const
{
    // Convert path to absolute
    FString fixedPath = AddTrailingSlash(nameIn);
    if (!IsAbsolutePath(fixedPath))
        fixedPath = ModuleManager::Get().FileSystemModule()->GetCurrentDir() + fixedPath;

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
	Font::RegisterObject();
}

}
