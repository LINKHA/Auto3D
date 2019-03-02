#include "UICamera.h"

namespace Auto3D
{

UICamera::UICamera()
{

}

UICamera::~UICamera()
{

}

void UICamera::RegisterObject()
{
	RegisterFactory<UICamera>();
}

}