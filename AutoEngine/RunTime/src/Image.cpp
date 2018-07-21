#include "Image.h"


AUTO_BEGIN



Image::Image()
{}
Image::Image(int width, int height)
{
	ImageReference::height = height;
	ImageReference::width = width;
}
Image::~Image()
{

}


AUTO_END