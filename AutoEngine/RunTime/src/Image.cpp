#include "Image.h"


namespace Auto3D {



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


}