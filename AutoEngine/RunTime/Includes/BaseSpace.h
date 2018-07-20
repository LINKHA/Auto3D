#pragma once
#include "Auto.h"
#include "ObjectDefines.h"
#include "Singleton.h"
#include "Mesh.h"
#include "Math/Math.h"
#include "Camera.h"
#include "LightManager.h"
USING_MATH
AUTO_BEGIN
/**
* @brief Singleton class
*/
class BaseSpace : public Singleton<BaseSpace>
{
public:
	BaseSpace();
	~BaseSpace();
	void Awake();
	void Start(); 
	void Update(); 
	void FixUpdate();
	void Finish();
	void Draw();
};

AUTO_END

