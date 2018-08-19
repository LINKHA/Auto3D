#include "Image.h"


namespace Auto3D {

Image::Image()
{
	value = NULL;
	width = 0;
	height = 0;
	format = 0;
}
Image::Image(int width, int height)
{
	value = NULL; 
	this->width = width;
	this->height = height;
	format = 0;
}



}