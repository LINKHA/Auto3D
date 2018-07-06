#include "MassInstantiationSpace.h"
#include "Application.h"
#include "BaseLight.h"
#include "FreeCamera.h"
#include "Mesh.h"
#include "Prefab.h"
#include "InstanceBeltLine.h"
MassInstantiationSpace::MassInstantiationSpace()
{
}
MassInstantiationSpace::~MassInstantiationSpace()
{
}

void MassInstantiationSpace::Awake()
{
	/*GameObject * prefabObj = new GameObject();
	Mesh * mesh = new Mesh();
	prefabObj->AddComponent(mesh);

	Prefab p(prefabObj);*/
}
void MassInstantiationSpace::Start()
{
	glm::mat4* modelMatrices;
	int amount = 10000;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); 
	float radius = 150.0;
	float offset = 25.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		modelMatrices[i] = model;
	}

	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->AddComponent(camera);
	
	GameObject * beltLineObj = new GameObject();
	InstanceBeltLine * line = 
		new InstanceBeltLine(ModelCommand("Resource/object/rock/rock.obj")
			,Shader("asteroids.auvs", "asteroids.aufs"),
			modelMatrices, amount);
	beltLineObj->AddComponent(line);
}
void MassInstantiationSpace::Update()
{

}
int MassInstantiationSpace::Launch()
{
	return INSTANCE(Application).Run();
}