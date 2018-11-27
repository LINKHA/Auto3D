#include "ResourceSystem.h"
#include "AutoOGL.h"
#include "AutoImage.h"
#include "tImage.h"
#include "Model.h"
#include "FileSystem.h"
#include "Resource.h"
#include "Sound.h"
#include "Sprite2D.h"
#include "Image.h"

#include "DebugNew.h"


namespace Auto3D {

static const SharedPtr<Resource> noResource;

ResourceSystem::ResourceSystem(Ambient* ambient)
	:Super(ambient)
{}

ResourceSystem::~ResourceSystem()
{}

void ResourceSystem::Init()
{
	auto* filesystem = GetSubSystem<FileSystem>();
	auto resourcePath = filesystem->GetParentPath(filesystem->GetCurrentDir());
	AddResourcePath(resourcePath + L"Resource/");
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

tImage* ResourceSystem::ImageLoad(PInt8 path)
{
	stbi_set_flip_vertically_on_load(true);
	tImage* image = nullptr;
	auto it = _imageQueue.find(path);
	if (it != _imageQueue.end())
	{
		image = it->second;
	}
	else
	{
		//image = new (std::nothrow)tImage(_ambient);
		image = new tImage(_ambient);
		int nrComponents;
		unsigned char * t = stbi_load(path, &image->width, &image->height, &nrComponents, 0);
		if (nrComponents == 1)
			image->format = GL_RED;
		else if (nrComponents == 3)
			image->format = GL_RGB;
		else if (nrComponents == 4)
			image->format = GL_RGBA;
		image->value = t;
		_imageQueue.emplace(MAKE_PAIR(path, image));

	}

	return image;
}

Model * ResourceSystem::ModelLoad(PInt8 path)
{
	Model* model;
	auto it = _modelQueue.find(path);
	if (it != _modelQueue.end())
	{
		model = it->second;
	}
	else
	{
		model = new Model(_ambient,path);
		_modelQueue.emplace(path, model);
	}
	return model;
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
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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

void ResourceSystem::FreeImage(tImage * image)
{
	stbi_image_free(image->value);
}

Resource* ResourceSystem::GetResource(STRING type, const STRING& name, bool sendEventOnFailure)
{

	return nullptr;
}

void ResourceSystem::AddResourcePath(const WSTRING& path)
{
	resourcePaths.push_back(path);
}

VECTOR<WSTRING> ResourceSystem::GetResourcePaths()
{
	return resourcePaths;
}

void ResourceSystem::RegisterResourceLib(Ambient * ambient)
{
	Sound::RegisterObject(ambient);
	Sprite2D::RegisterObject(ambient);
	Image::RegisterObject(ambient);
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

}