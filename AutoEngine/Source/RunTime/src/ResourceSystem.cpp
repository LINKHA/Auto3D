#include "ResourceSystem.h"
#include "AutoOGL.h"
#include "AutoImage.h"
#include "File.h"
#include "FileSystem.h"
#include "Resource.h"
#include "AudioBuffer.h"
#include "Image.h"
#include "Mesh.h"
#include "Shader.h"
#include "NewDef.h"


namespace Auto3D {

static const SharedPtr<Resource> noResource;

ResourceSystem::ResourceSystem(Ambient* ambient)
	:Super(ambient)
{
	RegisterResourceLib(ambient);
}

ResourceSystem::~ResourceSystem()
{}

void ResourceSystem::Init()
{
	auto* filesystem = GetSubSystem<FileSystem>();
	auto resourcePath = GetParentPath(filesystem->GetCurrentDir());
	//AddResourceDir(resourcePath + L"Resource/");
}

unsigned int ResourceSystem::TextureLoad(PInt8 path, bool vertically)
{
	stbi_set_flip_vertically_on_load(vertically);
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		WarningString("Texture failed to load at path");
		stbi_image_free(data);
	}
	return textureID;
}

unsigned int ResourceSystem::CubemapLoad(VECTOR<STRING> faces)
{
	stbi_set_flip_vertically_on_load(false);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].CStr(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			ErrorString("Cubemap texture failed to load at path: ");
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int ResourceSystem::HdrLoad(PInt8 path)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned int hdrTexture;
	int width, height, nrComponents;
	float *data = stbi_loadf(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		ErrorString("Failed to load HDR image.");
	}
	return hdrTexture;
}


Resource* ResourceSystem::GetResource(STRING type, const STRING& name, bool sendEventOnFailure)
{
	STRING sanitatedName = SanitateResourceName(name);

	// If empty name, return null pointer immediately
	if (sanitatedName.Empty())
		return nullptr;

	const SharedPtr<Resource>& existing = findResource(type, sanitatedName);
	if (existing)
		return existing.get();

	Resource* resource = dynamic_cast<Resource*>(_ambient->CreateObject(type));

	// Attempt to load the resource
	File* file = GetFile(sanitatedName, sendEventOnFailure);
	if (!file)
		return nullptr;   // Error is already logged
	resource->SetName(file->GetName());
	resource->Load(*file);

	return resource;
}

void ResourceSystem::AddResourceDir(const STRING& path, unsigned priority)
{
	_resourceDirs.push_back(path);
}

VECTOR<STRING> ResourceSystem::GetResourceDirs()
{
	return _resourceDirs;
}

File* ResourceSystem::GetFile(const STRING& name, bool sendEventOnFailure)
{
	STRING sanitatedName = SanitateResourceName(name);

	if (sanitatedName.Length())
	{
		File* file = nullptr;
		file = searchResourceDirs(sanitatedName);
		if (file)
			return file;
	}

	return nullptr;
}

STRING ResourceSystem::SanitateResourceName(const STRING& name) const
{
	// Sanitate unsupported constructs from the resource name
	STRING sanitatedName = GetInternalPath(name);
	
	sanitatedName.Replace("../", "");
	sanitatedName.Replace("./", "");

	auto* fileSystem = GetSubSystem<FileSystem>();
	if (_resourceDirs.size())
	{
		STRING namePath = GetPath(sanitatedName);
		STRING exePath = fileSystem->GetProgramDir().Replaced("/./", "/");
		for (unsigned i = 0; i < _resourceDirs.size(); ++i)
		{
			STRING relativeResourcePath = _resourceDirs[i];
			if (relativeResourcePath.StartsWith(exePath))
				relativeResourcePath = relativeResourcePath.SubString(exePath.Length());

			if (namePath.StartsWith(_resourceDirs[i], false))
				namePath = namePath.SubString(_resourceDirs[i].Length());
			else if (namePath.StartsWith(relativeResourcePath, false))
				namePath = namePath.SubString(relativeResourcePath.Length());
		}

		sanitatedName = namePath + GetFileNameAndExtension(sanitatedName);
	}

	return sanitatedName.Trimmed();
}

STRING ResourceSystem::GetResourceFileName(const STRING& name) const
{
	auto* fileSystem = GetSubSystem<FileSystem>();
	for (unsigned i = 0; i < _resourceDirs.size(); ++i)
	{
		if (fileSystem->FileExists(_resourceDirs[i] + name))
			return _resourceDirs[i] + name;
	}

	if (IsAbsolutePath(name) && fileSystem->FileExists(name))
		return name;
	else
		return STRING();
}

void ResourceSystem::RegisterResourceLib(Ambient* ambient)
{
	Image::RegisterObject(ambient);
	Mesh::RegisterObject(ambient);
	Shader::RegisterObject(ambient);
	AudioBuffer::RegisterObject(ambient);
}

const SharedPtr<Resource>& ResourceSystem::findResource(STRING type, STRING name)
{
	HASH_MAP<STRING, ResourceGroup>::iterator i = _resourceGroups.find(type);
	if (i == _resourceGroups.end())
		return noResource;

	HASH_MAP<STRING, SharedPtr<Resource> >::iterator j = i->second.resources.find(name);
	if (j == i->second.resources.end())
		return noResource;

	return j->second;
}

const SharedPtr<Resource>& ResourceSystem::findResource(STRING name)
{
	for (HASH_MAP<STRING, ResourceGroup>::iterator i = _resourceGroups.begin(); i != _resourceGroups.end(); ++i)
	{
		HASH_MAP<STRING, SharedPtr<Resource>>::iterator j = i->second.resources.find(name);
		if (j != i->second.resources.end())
			return j->second;
	}
	return noResource;
}

File* ResourceSystem::searchResourceDirs(const STRING& name)
{
	auto* fileSystem = GetSubSystem<FileSystem>();
	for (unsigned i = 0; i < _resourceDirs.size(); ++i)
	{
		if (fileSystem->FileExists(_resourceDirs[i] + name))
		{
			// Construct the file first with full path, then rename it to not contain the resource path,
			// so that the file's sanitatedName can be used in further GetFile() calls (for example over the network)
			File* file = new File(_ambient, _resourceDirs[i] + name);
			file->SetName(_resourceDirs[i] + name);
			return file;
		}
	}

	// Fallback using absolute path
	if (fileSystem->FileExists(name))
		return new File(_ambient, name);

	return nullptr;
}

}