#ifndef CLASS_ID_H_
#define CLASS_ID_H_
#define ClassID(x)		  CLASS_##x
#define DO_CLASS_ID(x,ID) ClassID(x) = ID,
enum ClassIDType
{
	DO_CLASS_ID(Default, -1)
	DO_CLASS_ID(Object, 0)

	DO_CLASS_ID(GameManager, 1)
	DO_CLASS_ID(LevelGameManager, 2)
	DO_CLASS_ID(GlobalGameManager, 3)
	DO_CLASS_ID(TimeManager, 4)
	DO_CLASS_ID(MotionManager,5)
	DO_CLASS_ID(RenderManager,7)
	DO_CLASS_ID(GameObjectManager, 8)
	DO_CLASS_ID(LightManager, 9)

	DO_CLASS_ID(Node, 13)
	DO_CLASS_ID(GameObject, 14)

	DO_CLASS_ID(Mesh, 15)
	DO_CLASS_ID(Texture, 16)
	DO_CLASS_ID(Texture2D, 17)
	DO_CLASS_ID(Light, 18)
	DO_CLASS_ID(Camera, 19)

	DO_CLASS_ID(Component, 20)
	DO_CLASS_ID(Transform, 21)

	DO_CLASS_ID(Motion,27)
	CLASS_ID_Count
};



#endif // !CLASS_ID_H_
