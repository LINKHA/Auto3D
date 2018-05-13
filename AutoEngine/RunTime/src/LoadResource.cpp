#include "LoadResource.h"

AUTO_BEGIN

AUTO_HASH_MAP(PInt8, Image*) imageQueue;
AUTO_HASH_MAP(PInt8, ModelCommand) modelQueue;

class Image;
class ModelCommand;
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
		
		imageRecord t = stbi_load(path, &image->Width, &image->Height, &image->Channels, 0);
		
		image->Value = t;
		imageQueue.emplace(M_PAIR(path, image));
		//stbi_image_free(t);
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
		modelQueue.emplace(M_PAIR(path, model));
	}
	return model;
}


AUTO_END


