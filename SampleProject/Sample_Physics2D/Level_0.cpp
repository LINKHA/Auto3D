#include "Level_0.h"
#include "GameObject.h"
#include "../FreeCamera.h"
#include "PhysicsWorld2D.h"
#include "RigidBody2D.h"
#include "ColliderBox2D.h"
Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void Level_0::Awake()
{
	SceneSuper::Awake();
}

void Level_0::Start()
{
	SceneSuper::Start();

	Node* groundNode = CreateNode();
	groundNode->SetPosition(0.0f, -10.0f);
	groundNode->CreateComponent<RigidBody2D>();
	groundNode->CreateComponent<ColliderBox2D>()->SetSize(100.0f, 20.0f);


	bodyNode = CreateNode();
	bodyNode->SetPosition(0.0f, 4.0f);
	bodyNode->CreateComponent<RigidBody2D>()->SetBodyType(BodyType2D::kDynamic);
	auto* bodyCollider = bodyNode->CreateComponent<ColliderBox2D>();
	bodyCollider->SetSize(2.0f, 2.0f);
	bodyCollider->SetDensity(1.0f);
	bodyCollider->SetFriction(0.3f);

}

void Level_0::Update()
{
	SceneSuper::Update();
	Vector3 pos = bodyNode->GetPosition();
	/*b2Body* body = bodyNode->GetComponent<RigidBody2D>()->GetBody();
	b2Vec2 position = body->GetPosition();
	float32 angle = body->GetAngle();
	printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);*/
	AutoCout << pos.x <<"   "<< pos.y << "   " << pos.z << AutoEndl;
}

