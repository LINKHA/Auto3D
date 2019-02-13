#include "Texture2D.h"

namespace Auto3D {


bool Texture2D::BeginLoad(Deserializer& source)
{
	// In headless mode, do not actually load the texture, just return success
	if (!_graphics.lock())
		return true;

	// If device is lost, retry later
	if (_graphics.lock()->IsDeviceLost())
	{
		WarningString("Texture load while device is lost");
		_dataPending = true;
		return true;
	}

	// Load the image data for EndLoad()
	_loadImage = MakeShared<Image>(_ambient);
	if (!_loadImage->Load(source))
	{
		_loadImage.reset();
		return false;
	}
	return true;
}

bool Texture2D::EndLoad()
{
	// In headless mode, do not actually load the texture, just return success
	if (!_graphics.lock() || _graphics.lock()->IsDeviceLost())
		return true;

	bool success = SetData(_loadImage);

	_loadImage.reset();

	return success;
}



}