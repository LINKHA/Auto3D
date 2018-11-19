#include "Level_0.h"
#include "AutoPhysics3D.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void Level_0::Awake()
{
	SceneSuper::Awake();
}

void Level_0::Start()
{
 //  ///-----initialization_start-----

 //  // 创建碰撞配置对象以及碰撞调度器对象,使我们可以再各个阶段尝试不同的算法组合,目的是使用不同的算法和测试相同的碰撞
 //  // 碰撞配置
 //  ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	//btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	//// 碰撞调度
	/////use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	//btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//// btDbvtBroadphase用来执行快速碰撞检测    目的是尽量的剔除没有相互作用的对象对
	/////btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	//btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	//// 实际上的物理模拟器 
	//// 创建解算器，用于求解约束方程。得到物体在重力等作用下的最终位置的
	/////the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	//btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	//// 独立场景动态世界
	//btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(
	//	dispatcher, overlappingPairCache, solver, collisionConfiguration);

	//dynamicsWorld->setGravity(btVector3(0, -10, 0));        // 设置重力加速度 Y向下

	///-----initialization_end-----

	//跟踪形状，在退出时释放内存。
	//尽可能重复使用刚体之间的碰撞形状!
	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	dynamicsWorld = GetSceneNode()->GetPhysicsWorld()->GetWorld();
	///create a few basic rigid bodies

	//创建一个地面,并加入到场景世界中
	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.))); // 长方体

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));        // 设置原点位置

		btScalar mass(0.);      // 质量

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);        // 惯性
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia); //通过质量，这个函数计算出运动物体的惯性

		// 运动状态
		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		// 刚体构造信息
		btRigidBody::btRigidBodyConstructionInfo  rbInfo(mass, myMotionState, groundShape, localInertia);
		// 刚体
		btRigidBody* body = new btRigidBody(rbInfo);

		// 将刚体添加至动态世界中
		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		//创建一个球体,并加入到场景世界中
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));       // 球体 半径为1
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(2, 10, 0));

		btScalar mass(1.f);     // 质量

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);       // 惯性
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia); //通过质量，这个函数计算出运动物体的惯性

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		//推荐使用motionstate，它提供插值功能，只同步“活动”对象
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}

	/// Do some simulation
	//int i = 0;
	// 步进模拟
	///-----stepsimulation_start-----
	//for (i = 0; i < 150; i++)
	//{
	//	// 模拟运动  每次进行物理模拟运算的时间间隔  每次能响应的最大step数
	//	//dynamicsWorld->stepSimulation(1.f / 5.f, 10);       

	//	//print positions of all objects
	//	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	//	{
	//		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
	//		btRigidBody* body = btRigidBody::upcast(obj);
	//		btTransform trans;
	//		if (body && body->getMotionState())
	//		{
	//			body->getMotionState()->getWorldTransform(trans);
	//		}
	//		else
	//		{
	//			trans = obj->getWorldTransform();
	//		}
	//		printf("world pos object %d = %f,%f,%f\n", j,
	//			float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	//	}
	//	printf(" \n");
	//}

	///-----stepsimulation_end-----

	//cleanup in the reverse order of creation/initialization

	///-----cleanup_start-----

	//清理动态世界里的刚体
	//remove the rigidbodies from the dynamics world and delete them
	/*for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}*/

	//清理碰撞形状
	//delete collision shapes
	/*for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}*/

	////清理动态世界
	////delete dynamics world
	//delete dynamicsWorld;

	////清理求解器
	////delete solver
	//delete solver;

	////清理粗测阶段
	////delete broadphase
	//delete overlappingPairCache;

	////清理调度
	////delete dispatcher
	//delete dispatcher;

	//delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	//collisionShapes.clear();

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

