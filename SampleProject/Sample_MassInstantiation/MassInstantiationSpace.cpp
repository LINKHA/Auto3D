#include "MassInstantiationSpace.h"
#include "Application.h"
#include "Light.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "Prefab.h"
#include "InstanceBeltLine.h"
#include "Math/Rand.h"
#include "Time.h"

MassInstantiationSpace::MassInstantiationSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
MassInstantiationSpace::~MassInstantiationSpace()
{
}

void MassInstantiationSpace::Awake()
{
}
void MassInstantiationSpace::Start()
{
	glm::mat4* modelMatrices;
	int amount = 100000;
	modelMatrices = new glm::mat4[amount];
	Rand rand(GetSubSystem<Time>()->GetRealTime().second);

	float radius = 150.0;
	float offset = 25.0f;
	for (int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand.GetInt() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand.GetInt() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand.GetInt() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		float scale = (rand.GetInt() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		float rotAngle = (rand.GetInt() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		modelMatrices[i] = model;
	}

	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->AddComponent(camera);
	
	GameObject * beltLineObj = new GameObject(_ambient);
	InstanceBeltLine * line = 
		new InstanceBeltLine(_ambient,ModelCommand("../Resource/object/rock/rock.obj")
			,Shader("asteroids.auvs", "asteroids.aufs"),
			modelMatrices, amount);
	beltLineObj->AddComponent(line);
}
void MassInstantiationSpace::Update()
{

}
int MassInstantiationSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(MassInstantiationSpace)