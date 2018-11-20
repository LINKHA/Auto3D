#include "Level_0.h"
#include "AutoPhysics3D.h"
#include "ColliderBox.h"
#include "ColliderSphere.h"
#include "RigidBody.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void Level_0::Awake()
{
	SceneSuper::Awake();
}

void Level_0::Start()
{
	dynamicsWorld = GetSceneNode()->GetPhysicsWorld()->GetWorld();

	Node* ground = CreateNode();
	ground->SetPosition(0, -56.0f, 0);
	auto* goundRig = ground->CreateComponent<RigidBody>();
	goundRig->SetMass(0);
	auto* box = ground->CreateComponent<ColliderBox>();
	box->SetSize(50.0f);

	Node* col = CreateNode();
	col->SetPosition(0.0f, 10.0f, 0);
	auto* goundRig2 = col->CreateComponent<RigidBody>();
	goundRig2->SetMass(1.0f);
	auto* sphere = col->CreateComponent<ColliderSphere>();


}

void Level_0::Update()
{
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		printf("world pos object %d = %f,%f,%f\n", j,
			float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}
	printf(" \n");
}

