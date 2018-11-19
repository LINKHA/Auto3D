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

 //  // ������ײ���ö����Լ���ײ����������,ʹ���ǿ����ٸ����׶γ��Բ�ͬ���㷨���,Ŀ����ʹ�ò�ͬ���㷨�Ͳ�����ͬ����ײ
 //  // ��ײ����
 //  ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	//btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	//// ��ײ����
	/////use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	//btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//// btDbvtBroadphase����ִ�п�����ײ���    Ŀ���Ǿ������޳�û���໥���õĶ����
	/////btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	//btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	//// ʵ���ϵ�����ģ���� 
	//// �������������������Լ�����̡��õ������������������µ�����λ�õ�
	/////the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	//btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	//// ����������̬����
	//btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(
	//	dispatcher, overlappingPairCache, solver, collisionConfiguration);

	//dynamicsWorld->setGravity(btVector3(0, -10, 0));        // �����������ٶ� Y����

	///-----initialization_end-----

	//������״�����˳�ʱ�ͷ��ڴ档
	//�������ظ�ʹ�ø���֮�����ײ��״!
	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	dynamicsWorld = GetSceneNode()->GetPhysicsWorld()->GetWorld();
	///create a few basic rigid bodies

	//����һ������,�����뵽����������
	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.))); // ������

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));        // ����ԭ��λ��

		btScalar mass(0.);      // ����

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);        // ����
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia); //ͨ���������������������˶�����Ĺ���

		// �˶�״̬
		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		// ���幹����Ϣ
		btRigidBody::btRigidBodyConstructionInfo  rbInfo(mass, myMotionState, groundShape, localInertia);
		// ����
		btRigidBody* body = new btRigidBody(rbInfo);

		// �������������̬������
		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		//����һ������,�����뵽����������
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));       // ���� �뾶Ϊ1
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(2, 10, 0));

		btScalar mass(1.f);     // ����

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);       // ����
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia); //ͨ���������������������˶�����Ĺ���

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		//�Ƽ�ʹ��motionstate�����ṩ��ֵ���ܣ�ֻͬ�����������
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	}

	/// Do some simulation
	//int i = 0;
	// ����ģ��
	///-----stepsimulation_start-----
	//for (i = 0; i < 150; i++)
	//{
	//	// ģ���˶�  ÿ�ν�������ģ�������ʱ����  ÿ������Ӧ�����step��
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

	//����̬������ĸ���
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

	//������ײ��״
	//delete collision shapes
	/*for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}*/

	////����̬����
	////delete dynamics world
	//delete dynamicsWorld;

	////���������
	////delete solver
	//delete solver;

	////����ֲ�׶�
	////delete broadphase
	//delete overlappingPairCache;

	////�������
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

