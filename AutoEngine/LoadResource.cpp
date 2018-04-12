#include "LoadResource.h"

AUTO_BEGIN

AUTO_HASH_MAP(PInt8, Image*) imageQueue;

class Image;

Image * localImageLoad(PInt8 path)
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
		imageRecord t = stbi_load(path, &image->m_Width, &image->m_Height, &image->m_Channels, 0);
		image->m_Image = t;
		imageQueue.emplace(M_PAIR(path, image));
		//stbi_image_free(t);
	}
	return image;
}
void freeImage(Image * image)
{
	stbi_image_free(image->m_Image);
}



AUTO_END


