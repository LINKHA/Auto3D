#include "Resource.h"
#include "GLGather.h"
#include "stb_image.h"
#include "Image.h"
#include "Model.h"
namespace Auto3D {

Resource::Resource(Ambient* ambient)
	:Super(ambient)
{}
Resource::~Resource()
{}

unsigned int Resource::TextureLoad(PInt8 path, bool vertically)
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

Image* Resource::ImageLoad(PInt8 path)
{
	stbi_set_flip_vertically_on_load(true);
	Image* image = nullptr;
	auto it = _imageQueue.Find(path);
	if (it != _imageQueue.End())
	{
		image = it->second;
	}
	else
	{
		//image = new (std::nothrow)Image(_ambient);
		image = new Image(_ambient);
		int nrComponents;
		unsigned char * t = stbi_load(path, &image->width, &image->height, &nrComponents, 0);
		if (nrComponents == 1)
			image->format = GL_RED;
		else if (nrComponents == 3)
			image->format = GL_RGB;
		else if (nrComponents == 4)
			image->format = GL_RGBA;
		image->value = t;
		//_imageQueue.Emplace(MAKE_PAIR(path, image));
		_imageQueue.Insert(MAKE_PAIR(path, image));
	}

	return image;
}

Model* Resource::ModelLoad(PInt8 path)
{
	Model* model;
	auto it = _modelQueue.Find(path);
	if (it != _modelQueue.End())
	{
		model = it->second;
	}
	else
	{
		model = new Model(_ambient,path);
		//_modelQueue.Emplace(path, model);
		_modelQueue.Insert(MAKE_PAIR(path, model));
	}
	return model;
}

unsigned int Resource::CubemapLoad(VECTOR(STRING) faces)
{
	stbi_set_flip_vertically_on_load(false);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.Size(); i++)
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

unsigned int Resource::HdrLoad(PInt8 path)
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

void Resource::FreeImage(Image * image)
{
	stbi_image_free(image->value);
}


}