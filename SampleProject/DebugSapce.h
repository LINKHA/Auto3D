#pragma  once
#include "MotionSpace.h"
USING_AUTO
class DebugSapce : public MotionSpace
{
public:
	DebugSapce();
	~DebugSapce();
	void Start()override;
	void Update()override;
	int Launch();
	Shader ourShader;
	unsigned int texture;
	unsigned int VBO, VAO, EBO;
};


