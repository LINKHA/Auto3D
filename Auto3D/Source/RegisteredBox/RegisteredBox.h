#pragma once

#include "Core/GameManager.h"
#include "AutoConfig.h"

namespace Auto3D
{

class AScene;
class ACamera;
class AScene2D;
class ACamera2D;

/// Manages messaging between the user and the engine
class AUTO_API ARegisteredBox : public ABaseModule
{
	REGISTER_OBJECT_CLASS(ARegisteredBox, ABaseModule)
public:
	/// Construct.
	ARegisteredBox();
	/// Destructor
	~ARegisteredBox();
	/// Register scene
	void RegisterScene(AScene* scene) { SetActiveScene(scene);  _scenes.Push(scene); }
	/// Register scene2d
	void RegisterScene2D(AScene2D* scene2d) { SetActiveScene2D(scene2d); _scene2ds.Push(scene2d); }
	/// Set active scene.
	void SetActiveScene(AScene* scene);
	/// Set active scene2d.
	void SetActiveScene2D(AScene2D* scene2d);

	/// Return scenes.
	const TVector<AScene*>& GetScenes() { return _scenes; }
	/// Return scene2d.
	const TVector<AScene2D*>& GetScene2Ds() { return _scene2ds; }
	/// Get active scenen.
	AScene* GetActiveScene();
	/// Get active scene2d.
	AScene2D* GetActiveScene2D();
private:
	/// AScene array.
	TVector<AScene*> _scenes;
	/// AScene2D array. 
	TVector<AScene2D*> _scene2ds;

	/// Active scene.
	AScene* _activeScene;
	/// Active scene2d.
	AScene2D* _activeScene2d;
};

}