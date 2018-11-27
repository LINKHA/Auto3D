#include "tImage.h"
#include "DebugNew.h"


namespace Auto3D {

tImage::tImage(Ambient* ambient)
	:Super(ambient)
	,width(0)
	,height(0)
	,format(0)
{
}
tImage::tImage(Ambient* ambient,int Width, int Height)
	:Super(ambient)
	,width(Width)
	,height(Height)
	,format(0)
{
}
tImage::~tImage()
{}


}