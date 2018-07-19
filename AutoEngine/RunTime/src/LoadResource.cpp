#include "LoadResource.h"

AUTO_BEGIN

AUTO_HASH_MAP(PInt8, Image*) imageQueue;
AUTO_HASH_MAP(PInt8, ModelCommand) modelQueue;

class Image;
class ModelCommand;
unsigned int LocalTextureLoad(PInt8 path)
{
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
Image* LocalImageLoad(PInt8 path)
{
	Image* image = nullptr;
	auto it = imageQueue.find(path);
	if (it != imageQueue.end()) 
	{
		image = (imageQueue.find(path))->second;
	}
	else
	{
		image = new (std::nothrow)Image();
		int nrComponents;
		unsigned char * t = stbi_load(path, &image->Width, &image->Height, &nrComponents, 0);
		if (nrComponents == 1)
			image->Format = GL_RED;
		else if (nrComponents == 3)
			image->Format = GL_RGB;
		else if (nrComponents == 4)
			image->Format = GL_RGBA;
		image->Value = t;
		imageQueue.emplace(M_PAIR(path, image));	
		
	}
	
	return image;
}
void FreeImage(Image * image)
{
	stbi_image_free(image->Value);
}

ModelCommand LocalModelLoad(PInt8 path)
{
	ModelCommand model;
	auto it = modelQueue.find(path);
	if (it != modelQueue.end())
	{
		model = (modelQueue.find(path))->second;
	}
	else
	{
		model = ModelCommand(path);
		modelQueue.emplace(path, model);
	}
	return model;
}

unsigned int LoadCubemap(_VECTOR(_String) faces)
{
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
unsigned int LocalHdrLoad(PInt8 path)
{
	unsigned int hdrTexture;
	int width, height, nrComponents;
	float *data = stbi_loadf("Resource/texture/hdr/Alexs_Apt_2k.hdr", &width, &height, &nrComponents, 0);
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
AUTO_END


