#pragma once
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

	DO_CLASS_ID(RenderManager, 7)
	DO_CLASS_ID(GameObjectManager, 8)
	DO_CLASS_ID(LightManager, 9)
	DO_CLASS_ID(ScriptManager, 10)
	DO_CLASS_ID(PreLoadManager, 11)

	DO_CLASS_ID(Node, 13)
	DO_CLASS_ID(GameObject, 14)

	DO_CLASS_ID(Component, 15)
	DO_CLASS_ID(Transform, 16)
	DO_CLASS_ID(Mesh, 17)
	DO_CLASS_ID(Texture, 18)
	DO_CLASS_ID(Texture2D, 19)
	DO_CLASS_ID(Light, 20)
	DO_CLASS_ID(Camera, 21)
	DO_CLASS_ID(ScriptComponent, 22)
	DO_CLASS_ID(Texture3D, 23)
	DO_CLASS_ID(SkyBox, 24)


	DO_CLASS_ID(MotionSpace, 30)
	DO_CLASS_ID(Prefab, 31)

	DO_CLASS_ID(TextureMap, 32)
	DO_CLASS_ID(TextureNormal, 33)
	DO_CLASS_ID(TextureParallax, 34)
	DO_CLASS_ID(RenderComponent, 35)
	DO_CLASS_ID(Sprite, 36)

	DO_CLASS_ID(LightPoint, 37)
	DO_CLASS_ID(LightDirectional, 38)
	DO_CLASS_ID(LightSpot, 39)

	DO_CLASS_ID(Engine,40)
	DO_CLASS_ID(Graphics,41)


	DO_CLASS_ID(debug_test, 100)
	
	CLASS_ID_Count
};

