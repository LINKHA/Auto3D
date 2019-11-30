#pragma once
#include "Collider.h"

namespace Auto3D 
{

class AUTO_API ColliderBox : public Collider
{
	REGISTER_OBJECT_CLASS(ColliderBox, Collider)
public:
	/// Construct
	ColliderBox();
	/// Destructer
	~ColliderBox();
	/// Register object factory.
	static void RegisterObject();
	/// Set size form TVector3
	void SetSize(const TVector3F& vec);
	/// Set size form float x y z
	void SetSize(float x, float y, float z);
	/// Set size form float
	void SetSize(float scale);
private:
	/// Resize form TVector3
	void Resize(const TVector3F& vec);

	/// Box shape size (radius)
	TVector3F _size;
};

}