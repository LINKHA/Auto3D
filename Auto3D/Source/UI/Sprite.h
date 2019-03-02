#pragma once
#include "UISpatialNode.h"

namespace Auto3D 
{
class Texture;

class AUTO_API Sprite : public UISpatialNode
{
	REGISTER_OBJECT_CLASS(Sprite, UISpatialNode)

public:
	Sprite();
	~Sprite();
	/// Register factory and attributes.
	static void RegisterObject();
	/// Set texture.
	void SetTexture(Texture* texture);
	/// Return texture.
	Texture* GetTexture() const { return _texture; }
private:
	/// Texture.
	SharedPtr<Texture> _texture;

};

}