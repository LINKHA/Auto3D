#include "Level_0.h"
#include "Node.h"
#include "../FreeCamera2D.h"
#include "PhysicsWorld2D.h"
#include "RigidBody2D.h"
#include "ColliderBox2D.h"
#include "ResourceSystem.h"
#include "SpriteRenderer.h"


void Level_0::Awake()
{
	Super::Awake();
}

void Level_0::Start()
{
	Super::Start();

	groundNode = CreateNode();
	groundNode->SetPosition(0.0f,-10.0f);
	groundNode->CreateComponent<RigidBody2D>();
	groundNode->CreateComponent<ColliderBox2D>()->SetSize(50.0f, 10.0f);


	bodyNode = CreateNode();
	bodyNode->SetPosition(0.0f, 4.0f);
	bodyNode->CreateComponent<RigidBody2D>()->SetBodyType(BodyType2D::kDynamic);
	auto bodyCollider = bodyNode->CreateComponent<ColliderBox2D>();
	bodyCollider->SetSize(2.0f, 2.0f);
	bodyCollider->SetDensity(1.0f);
	bodyCollider->SetFriction(0.3f);

}

void Level_0::Update()
{
	Super::Update();
	Vector3 groundPos = groundNode->GetPosition();
	Vector3 pos = bodyNode->GetPosition();
	AutoCout << groundPos.x << "   " << groundPos.y << "   " << groundPos.z << AutoEndl;
	AutoCout << pos.x <<"   "<< pos.y << "   " << pos.z << AutoEndl;
}

