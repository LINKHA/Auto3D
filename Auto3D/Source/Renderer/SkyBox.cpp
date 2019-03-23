#include "SkyBox.h"
#include "../Resource/Image.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

SkyBoxBuffer::SkyBoxBuffer()
{
	_data.Resize(0);
}

SkyBoxBuffer::SkyBoxBuffer(Image* right, Image* left, Image* top, Image*  bottom, Image* front, Image* back)
{
	_data.Resize(6);
	_data[0] = right;
	_data[1] = left;
	_data[2] = top;
	_data[3] = bottom;
	_data[4] = front;
	_data[5] = back;
}

SkyBoxBuffer::SkyBoxBuffer(const Vector<AutoPtr<Image> >& vec)
{
	_data.Resize(0);
	for (int i = 0; i < vec.Size() && i < 6; ++i)
	{
		_data.Push(vec[i]);
	}
}


SkyBox::SkyBox()
{

}

SkyBox::~SkyBox()
{

}

void SkyBox::RegisterObject()
{
	RegisterFactory<SkyBox>();
}

void SkyBox::SetImage(Image* right, Image* left, Image* top, Image*  bottom, Image* front, Image* back)
{
	_buffer = new SkyBoxBuffer(right, left, top, bottom, front, back);
}

/// Set the image resource with skybox buffer
void SkyBox::SetImage(SkyBoxBuffer* buffer)
{
	_buffer = buffer;
}


}