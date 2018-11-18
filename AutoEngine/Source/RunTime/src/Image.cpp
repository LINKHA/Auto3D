#include "Image.h"


namespace Auto3D {

Image::Image(Ambient* ambient)
	:Super(ambient)
	,width(0)
	,height(0)
	,format(0)
{
}
Image::Image(Ambient* ambient,int Width, int Height)
	:Super(ambient)
	,width(Width)
	,height(Height)
	,format(0)
{
}
Image::~Image()
{}


}