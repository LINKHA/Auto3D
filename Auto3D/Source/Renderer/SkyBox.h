#pragma once
#include "../Renderer/GeometryNode.h"

namespace Auto3D
{

class Image;
/// Skybox data buffering
struct SkyBoxBuffer
{
	/// Construct
	SkyBoxBuffer();
	/// According to each picture
	SkyBoxBuffer(Image* right, Image* left, Image* top, Image*  bottom, Image* front, Image* back);
	/// Based on the buffer array
	SkyBoxBuffer(const Vector<AutoPtr<Image> >& vec);
	/// Skybox data
	Vector<AutoPtr<Image> > _data;
};

// Relative to the three - dimensional space independent skybox
class AUTO_API SkyBox : public GeometryNode
{
	REGISTER_OBJECT_CLASS(SkyBox, Node)
public:
	/// Construct
	SkyBox();
	/// Destruct
	~SkyBox();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Set the image resource.
	void SetImage(Image* right, Image* left, Image* top, Image*  bottom, Image* front, Image* back);
	/// Set the image resource with skybox buffer
	void SetImage(SkyBoxBuffer* buffer);
	/// Update Cube texture
	void UpdateTexture(SkyBoxBuffer* buffer);
private:
	/// Skybox buffer
	AutoPtr<SkyBoxBuffer> _buffer;
	/// Cube texture
	AutoPtr<Texture> _texture;
};

}

