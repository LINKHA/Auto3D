#pragma once
#include "Collider.h"

namespace Auto3D 
{

class AUTO_API AColliderBox : public ACollider
{
	DECLARE_A_CLASS(AColliderBox, ACollider)
public:
	/// Construct
	AColliderBox();
	/// Destructer
	~AColliderBox();

	/// Set size form TVector3
	void SetSize(const TVector3F& vec);
	/// Set size form float x y z
	void SetSize(float x, float y, float z);
	/// Set size form float
	void SetSize(float scale);

	const TVector3F& GetSize() { return _size; }
private:
	/// Resize form TVector3
	void Resize(const TVector3F& vec);

	/// Box shape size (radius)
	TVector3F _size;
};

}