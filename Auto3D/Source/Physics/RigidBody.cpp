//#include "RigidBody.h"
//#include "PhysicsWorld.h"
//#include "PhysicsUtils.h"
//
//#include "../Scene/Scene.h"
//
//namespace Auto3D 
//{
//
//RigidBody::RigidBody():
//	_mass(0.0f),
//	_isDynamic(false),
//	_isDirty(false)
//{
//#if DebugCompoundShape
//	_compoundShape = new btCompoundShape();
//#else
//	_shape = nullptr;
//#endif
//}
//
//
//RigidBody::~RigidBody()
//{
//	SafeDelete(_motionState);
//#if DebugCompoundShape
//	SafeDelete(_compoundShape);
//#endif
//}
//
//void RigidBody::RegisterObject()
//{
//	RegisterFactory<RigidBody>();
//}
//
//void RigidBody::AddBodyToWorld()
//{
//}
//
//void RigidBody::UpdateMass()
//{
//
//}
//
//void RigidBody::UpdateGravity()
//{
//
//}
//
////
////void RigidBody::Update()
////{
////	//The reason it only runs once here is that you have to wait until all 
////	//	the collider Start functions are finished before you can register
////	if (_isFirstUpdate)
////	{
////		_isFirstUpdate = false;
////		RegisteredRigidBody();
////	}
////
////
////	UpdateMass();
////	UpdateGravity();
////}
//
//#if DebugCompoundShape
//void RigidBody::registeredRigidBody()
//{
//	_isDynamic = (_mass != 0.0f);
//	_physicsWorld = GetCurrentSceneNode()->GetPhysicsWorld();
//
//	Vector3 position = GetNode()->GetComponent<Transform>()->GetPosition();
//	btTransform groundTransform;
//	groundTransform.setIdentity();
//	groundTransform.setOrigin(ToBtVector3(position));
//
//	btVector3 localInertia(0, 0, 0);
//	if (_isDynamic)
//		_compoundShape->calculateLocalInertia(_mass, localInertia);
//
//	AutoCout << _compoundShape->getNumChildShapes() << AutoEndl;
//	_motionState = new btDefaultMotionState(groundTransform);
//
//	_body = new btRigidBody(_mass, _motionState, _compoundShape, localInertia);
//
//	_physicsWorld->GetWorld()->addRigidBody(_body);
//}
//#else
//void RigidBody::RegisteredRigidBody()
//{
//	/*_isDynamic = (_mass != 0.0f);
//	_physicsWorld = GetCurrentSceneNode()->GetComponent<PhysicsWorld>();
//
//	Vector3F position = GetNode()->GetComponent<Transform>()->GetPosition();
//	btTransform groundTransform;
//	groundTransform.setIdentity();
//	groundTransform.setOrigin(ToBtVector3(position));
//
//	btVector3 localInertia(0, 0, 0);
//	if (_isDynamic)
//		_shape->calculateLocalInertia(_mass, localInertia);
//
//	_motionState = new btDefaultMotionState(groundTransform);
//
//	_body = new btRigidBody(_mass, _motionState, _shape, localInertia);
//
//	_physicsWorld->GetWorld()->addRigidBody(_body);*/
//}
//#endif
//
//void RigidBody::ParentCallBack()
//{
//	_physicsWorld = ParentScene()->GetPhysicsWorld();
//}
//
//}