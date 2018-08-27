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
	DO_CLASS_ID(Time, 4)

	DO_CLASS_ID(GameObjectManager, 8)
	DO_CLASS_ID(LightManager, 9)
	DO_CLASS_ID(Script, 10)
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
	DO_CLASS_ID(Renderer, 42)
	DO_CLASS_ID(BaseSpace, 43)
	DO_CLASS_ID(RenderLoop, 44)
	DO_CLASS_ID(Input,45)
	DO_CLASS_ID(ComponentSetting, 46)
	DO_CLASS_ID(SpriteSort, 47)
	DO_CLASS_ID(Geometry, 48)
	DO_CLASS_ID(FrameBuffersScreen, 49)
	DO_CLASS_ID(MSAA, 50)
	DO_CLASS_ID(ShadowSetting, 51)
	DO_CLASS_ID(ShadowRenderAssist, 52)
	DO_CLASS_ID(ShadowRenderer, 53)
	DO_CLASS_ID(LightContainer, 54)
	DO_CLASS_ID(HDR, 55)
	DO_CLASS_ID(OffScreen, 56)
	DO_CLASS_ID(MeshPBR,57)
	DO_CLASS_ID(MeshPBRTexture, 58)
	DO_CLASS_ID(Image, 59)
	DO_CLASS_ID(Resource,60)
	DO_CLASS_ID(Material,61)
	DO_CLASS_ID(Scene, 62)
	DO_CLASS_ID(Hardware, 63)
	
	CLASS_ID_Count
};

