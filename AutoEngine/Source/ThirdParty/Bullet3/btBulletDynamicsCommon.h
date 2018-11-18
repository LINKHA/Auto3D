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

#ifndef BULLET_DYNAMICS_COMMON_H
#define BULLET_DYNAMICS_COMMON_H

///Common headerfile includes for Bullet Dynamics, including Collision Detection
#include "btBulletCollisionCommon.h"

#include "Bullet3/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

#include "Bullet3/BulletDynamics/Dynamics/btSimpleDynamicsWorld.h"
#include "Bullet3/BulletDynamics/Dynamics/btRigidBody.h"

#include "Bullet3/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "Bullet3/BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "Bullet3/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h"
#include "Bullet3/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "Bullet3/BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include "Bullet3/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h"
#include "Bullet3/BulletDynamics/ConstraintSolver/btUniversalConstraint.h"
#include "Bullet3/BulletDynamics/ConstraintSolver/btHinge2Constraint.h"
#include "Bullet3/BulletDynamics/ConstraintSolver/btGearConstraint.h"
#include "Bullet3/BulletDynamics/ConstraintSolver/btFixedConstraint.h"

#include "Bullet3/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"

///Vehicle simulation, with wheel contact simulated by raycasts
#include "Bullet3/BulletDynamics/Vehicle/btRaycastVehicle.h"

#endif  //BULLET_DYNAMICS_COMMON_H
