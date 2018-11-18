/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BULLET_COLLISION_COMMON_H
#define BULLET_COLLISION_COMMON_H

///Common headerfile includes for Bullet Collision Detection

///Bullet's btCollisionWorld and btCollisionObject definitions
#include "Bullet3/BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "Bullet3/BulletCollision/CollisionDispatch/btCollisionObject.h"

///Collision Shapes
#include "Bullet3/BulletCollision/CollisionShapes/btBoxShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btSphereShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btCylinderShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btConeShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "Bullet3/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "Bullet3/BulletCollision/CollisionShapes/btCompoundShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btTetrahedronShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btEmptyShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "Bullet3/BulletCollision/CollisionShapes/btUniformScalingShape.h"

///Narrowphase Collision Detector
#include "Bullet3/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"

//#include "Bullet3/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h"
#include "Bullet3/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"

///Dispatching and generation of collision pairs (broadphase)
#include "Bullet3/BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "Bullet3/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h"
#include "Bullet3/BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "Bullet3/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"

///Math library & Utils
#include "Bullet3/LinearMath/btQuaternion.h"
#include "Bullet3/LinearMath/btTransform.h"
#include "Bullet3/LinearMath/btDefaultMotionState.h"
#include "Bullet3/LinearMath/btQuickprof.h"
#include "Bullet3/LinearMath/btIDebugDraw.h"
#include "Bullet3/LinearMath/btSerializer.h"

#endif  //BULLET_COLLISION_COMMON_H
